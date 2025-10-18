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

// Callback que será chamada quando os dados ESP-NOW forem recebidos
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  // Ação Imediata: Copia os dados recebidos para a struct 'dados'
  memcpy(&dados, incomingData, sizeof(dados));

  // Ação de Log (SÓ IMPRIME QUANDO RECEBE DADOS)
  Serial.print("[NOW_RECV] ID Transmissor: ");
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
  pinMode(LED_ALARME, OUTPUT);
  digitalWrite(LED_ALARME, LOW);

  Serial.println("SLEEP HELP - Receptor ESP-NOW");
  
  // Força WiFi em modo STA para usar o ESP-NOW
  WiFi.mode(WIFI_STA);
  // O esp_wifi_start() é redundante se o WiFi.begin() ou WiFi.mode() for chamado, 
  // mas é mantido aqui para garantir o estado.
  esp_wifi_start(); 

  // Exibe o MAC deste receptor
  uint8_t mac[6];
  esp_wifi_get_mac(WIFI_IF_STA, mac);

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  Serial.print("MAC deste ESP32 (Receiver): ");
  Serial.println(macStr);
  Serial.println("========================================");


  if (esp_now_init() != ESP_OK) {
    Serial.println("[NOW] ❌ Erro ao iniciar ESP-NOW");
    return;
  }

  // Registra a função de callback para receber dados
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("[NOW] Pronto para receber comandos de alarme.");
}

void loop() {
  // O loop fica livre. A lógica de recepção, atuação e impressão está no callback.
}