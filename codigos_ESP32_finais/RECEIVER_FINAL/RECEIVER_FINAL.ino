#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h> 

#define LED_ALARME 13 // Pino onde o LED/Buzzer de alarme está conectado

// Struct deve ser idêntica à do transmissor
typedef struct {
  int id;
  bool ledOn; 
} mensagem;

mensagem dados;

// Variáveis para controle
int contadorRecebidos = 0;

// Callback de envio (para confirmar ACK quando receptor envia dados)
void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("[NOW_SEND] ✅ ACK enviado com sucesso");
  } else {
    Serial.println("[NOW_SEND] ❌ Falha ao enviar ACK");
  }
}

// Callback que será chamada quando os dados ESP-NOW forem recebidos
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  // Ação Imediata: Copia os dados recebidos para a struct 'dados'
  memcpy(&dados, incomingData, sizeof(dados));
  
  contadorRecebidos++;

  // Log do MAC do transmissor que enviou
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           info->src_addr[0], info->src_addr[1], info->src_addr[2],
           info->src_addr[3], info->src_addr[4], info->src_addr[5]);

  // Ação de Log (SÓ IMPRIME QUANDO RECEBE DADOS)
  Serial.print("[NOW_RECV] #");
  Serial.print(contadorRecebidos);
  Serial.print(" | MAC TX: ");
  Serial.print(macStr);
  Serial.print(" | ID: ");
  Serial.print(dados.id);
  Serial.print(" | Status: ");
  Serial.println(dados.ledOn ? "🚨 ALERTA" : "✅ OK");

  // Ação de Atuação (SÓ ATUA QUANDO RECEBE DADOS)
  if (dados.ledOn) {
    digitalWrite(LED_ALARME, HIGH);
  } else {
    digitalWrite(LED_ALARME, LOW);
  }
  
  // O ESP-NOW envia ACK automaticamente quando recebe dados
  // Não precisa fazer nada - o ACK é enviado automaticamente pelo protocolo
}

// Configurações de rede (mesmas do transmissor para descobrir o canal correto)
const char* ssid = "STREET_WIFI_SM"; 
const char* password = "StreetFI";

// Função para descobrir o canal WiFi conectando à mesma rede do transmissor
int descobrirCanalWiFi() {
  Serial.println("[SCAN] 🔍 Conectando ao WiFi para descobrir canal...");
  
  // Tenta conectar ao WiFi (mesma rede do transmissor)
  WiFi.begin(ssid, password);
  
  int tentativas = 0;
  Serial.print("[WIFI] Conectando");
  while (WiFi.status() != WL_CONNECTED && tentativas < 10) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    int canal = WiFi.channel();
    Serial.println();
    Serial.printf("[WIFI] ✅ Conectado! Canal descoberto: %d\n", canal);
    WiFi.disconnect(); // Desconecta após descobrir o canal
    delay(100);
    return canal;
  } else {
    Serial.println();
    Serial.println("[WIFI] ⚠️  Não conseguiu conectar, escaneando redes...");
    
    // Fallback: escaneia redes para descobrir canal
    int n = WiFi.scanNetworks();
    if (n == 0) {
      Serial.println("[SCAN] ⚠️  Nenhuma rede encontrada, usando canal 1");
      return 1;
    }
    
    // Procura especificamente pela rede do transmissor
    for (int i = 0; i < n; i++) {
      if (WiFi.SSID(i) == ssid) {
        int canal = WiFi.channel(i);
        Serial.printf("[SCAN] ✅ Rede '%s' encontrada no canal %d\n", ssid, canal);
        return canal;
      }
    }
    
    // Se não encontrou a rede específica, usa o canal mais comum
    int canais[14] = {0};
    for (int i = 0; i < n; i++) {
      int canal = WiFi.channel(i);
      if (canal >= 1 && canal <= 13) {
        canais[canal]++;
      }
    }
    
    int canalMaisComum = 1;
    int maxRedes = canais[1];
    for (int i = 2; i <= 13; i++) {
      if (canais[i] > maxRedes) {
        maxRedes = canais[i];
        canalMaisComum = i;
      }
    }
    
    Serial.printf("[SCAN] ✅ Canal mais comum: %d\n", canalMaisComum);
    return canalMaisComum;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(LED_ALARME, OUTPUT);
  digitalWrite(LED_ALARME, LOW);

  Serial.println("\n========================================");
  Serial.println("  SLEEP HELP - Receptor ESP-NOW");
  Serial.println("========================================");
  
  // Força WiFi em modo STA para usar o ESP-NOW
  WiFi.mode(WIFI_STA);
  
  // Descobre o canal WiFi automaticamente (uma vez só)
  int canalWiFi = descobrirCanalWiFi();
  
  // Configura o canal WiFi descoberto
  if (esp_wifi_set_channel(canalWiFi, WIFI_SECOND_CHAN_NONE) == ESP_OK) {
    Serial.printf("[WIFI] ✅ Canal WiFi: %d\n", canalWiFi);
  } else {
    Serial.println("[WIFI] ⚠️ Aviso: Falha ao configurar canal");
  }
  
  // Desativa power save para melhor recepção
  if (esp_wifi_set_ps(WIFI_PS_NONE) == ESP_OK) {
    Serial.println("[WIFI] ✅ Power Save desativado");
  }
  
  // Inicia WiFi
  esp_wifi_start();

  // Exibe o MAC deste receptor
  uint8_t mac[6];
  esp_wifi_get_mac(WIFI_IF_STA, mac);

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  Serial.print("[RECEIVER] MAC Address: ");
  Serial.println(macStr);
  Serial.println("========================================");

  // Inicializa ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("[NOW] ❌ Erro ao iniciar ESP-NOW");
    Serial.println("[NOW] Tente reiniciar o ESP32");
    return;
  }

  // IMPORTANTE: Receptor não precisa adicionar peer para receber
  // O ESP-NOW funciona em modo promiscuous - recebe automaticamente de qualquer transmissor no mesmo canal
  // O ACK é enviado automaticamente pelo protocolo ESP-NOW quando recebe dados

  // Registra callbacks
  esp_now_register_recv_cb(OnDataRecv);  // Callback de recepção
  esp_now_register_send_cb(OnDataSent); // Callback de envio (para logs de ACK)

  Serial.println("[NOW] ✅ ESP-NOW inicializado");
  Serial.println("[NOW] 📡 Pronto para receber comandos");
  Serial.println("[NOW] 💡 Recebendo de qualquer transmissor no canal " + String(canalWiFi));
  Serial.println("========================================\n");
}

void loop() {
  // O loop fica livre. A lógica de recepção está no callback OnDataRecv
  // Não fica tentando canais - configura uma vez e funciona!
  delay(100);
}