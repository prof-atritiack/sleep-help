#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h> 

#define LED_ALARME 13 // Pino onde o LED/Buzzer de alarme está conectado

// ⚠️ IMPORTANTE: Configure o canal WiFi aqui (deve ser o mesmo do transmissor)
// O transmissor imprime o canal no Serial quando conecta ao WiFi
// Exemplo: "[WIFI/NOW] Canal sincronizado para 6"
// Se não souber, deixe 1 e veja o Serial do transmissor para descobrir
#define WIFI_CHANNEL 1  // ⚠️ ALTERE PARA O CANAL DO TRANSMISSOR!

// Struct deve ser idêntica à do transmissor
typedef struct {
  int id;
  bool ledOn; 
} mensagem;

mensagem dados;

// Variáveis para controle
unsigned long ultimaRecepcao = 0;
int contadorRecebidos = 0;

// Callback que será chamada quando os dados ESP-NOW forem recebidos
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  // Ação Imediata: Copia os dados recebidos para a struct 'dados'
  memcpy(&dados, incomingData, sizeof(dados));
  
  ultimaRecepcao = millis();
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
  
  // Configura o canal WiFi (CRÍTICO para ESP-NOW funcionar)
  if (esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE) == ESP_OK) {
    Serial.printf("[WIFI] ✅ Canal WiFi configurado: %d\n", WIFI_CHANNEL);
  } else {
    Serial.println("[WIFI] ⚠️ Aviso: Falha ao configurar canal (pode funcionar mesmo assim)");
  }
  
  // Desativa power save para melhor recepção
  if (esp_wifi_set_ps(WIFI_PS_NONE) == ESP_OK) {
    Serial.println("[WIFI] ✅ Power Save desativado para melhor recepção");
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

  // Registra a função de callback para receber dados
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("[NOW] ✅ ESP-NOW inicializado com sucesso");
  Serial.println("[NOW] 📡 Pronto para receber comandos de alarme");
  Serial.println("");
  Serial.println("⚠️  TROUBLESHOOTING:");
  Serial.println("   1. Verifique o canal WiFi (atual: " + String(WIFI_CHANNEL) + ")");
  Serial.println("   2. Veja o Serial do TRANSMISSOR para descobrir o canal");
  Serial.println("   3. Procure por: '[WIFI/NOW] Canal sincronizado para X'");
  Serial.println("   4. Altere WIFI_CHANNEL no receptor para o mesmo valor");
  Serial.println("   5. Certifique-se de que ambos estão ligados e próximos");
  Serial.println("========================================\n");
}

void loop() {
  // Monitora se está recebendo dados
  if (ultimaRecepcao > 0 && (millis() - ultimaRecepcao) > 30000) {
    // Se não recebeu nada nos últimos 30 segundos, avisa
    static unsigned long ultimoAviso = 0;
    if (millis() - ultimoAviso > 60000) { // Avisa a cada 60 segundos
      Serial.println("[NOW] ⚠️  Nenhum dado recebido há mais de 30 segundos");
      Serial.println("[NOW]    Verifique: canal WiFi, distância, alimentação");
      ultimoAviso = millis();
    }
  }
  
  delay(100); // Pequeno delay para não sobrecarregar
}