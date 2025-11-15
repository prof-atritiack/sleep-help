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

// Callback que será chamada quando os dados ESP-NOW forem recebidos
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  // Ação Imediata: Copia os dados recebidos para a struct 'dados'
  memcpy(&dados, incomingData, sizeof(dados));
  
  contadorRecebidos++;

  // Ação de Log (SÓ IMPRIME QUANDO RECEBE DADOS)
  Serial.print("[NOW_RECV] #");
  Serial.print(contadorRecebidos);
  Serial.print(" | ID Transmissor: ");
  Serial.print(dados.id);
  Serial.print(" | Status Alarme: ");
  Serial.println(dados.ledOn ? "🚨 LIGADO (ALERTA)" : "✅ DESLIGADO (OK)");

  // Ação de Atuação (SÓ ATUA QUANDO RECEBE DADOS)
  if (dados.ledOn) {
    digitalWrite(LED_ALARME, HIGH);
  } else {
    digitalWrite(LED_ALARME, LOW);
  }
}

// Função para descobrir o canal WiFi automaticamente (executa apenas uma vez)
int descobrirCanalWiFi() {
  Serial.println("[SCAN] 🔍 Escaneando redes WiFi...");
  
  int n = WiFi.scanNetworks();
  int canais[14] = {0}; // Contador de redes por canal (1-13)
  
  if (n == 0) {
    Serial.println("[SCAN] ⚠️  Nenhuma rede encontrada, usando canal 1");
    return 1; // Retorna canal padrão
  }
  
  // Conta quantas redes estão em cada canal
  for (int i = 0; i < n; i++) {
    int canal = WiFi.channel(i);
    if (canal >= 1 && canal <= 13) {
      canais[canal]++;
    }
  }
  
  // Encontra o canal com mais redes (mais provável de ser usado)
  int canalMaisComum = 1;
  int maxRedes = canais[1];
  for (int i = 2; i <= 13; i++) {
    if (canais[i] > maxRedes) {
      maxRedes = canais[i];
      canalMaisComum = i;
    }
  }
  
  Serial.printf("[SCAN] ✅ Canal configurado: %d\n", canalMaisComum);
  return canalMaisComum;
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
  // Apenas registra o callback e pronto!

  // Registra a função de callback para receber dados
  esp_now_register_recv_cb(OnDataRecv);

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