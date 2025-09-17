#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>   // necessário para esp_wifi_start() e esp_wifi_get_mac()

#define LED 13

typedef struct {
  int id;
  bool ledOn;
} mensagem;

mensagem dados;

// Callback ESP-NOW (core 3.x)
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&dados, incomingData, sizeof(dados));

  Serial.print("ID: ");
  Serial.print(dados.id);
  Serial.print(" | LED: ");
  Serial.println(dados.ledOn ? "ON" : "OFF");

  digitalWrite(LED, dados.ledOn ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  Serial.println("SLEEP HELP - 3CT");
  delay(2000);

  // Força WiFi em modo STA
  WiFi.mode(WIFI_STA);
  esp_wifi_start();

  // Pega MAC da interface STA diretamente
  uint8_t mac[6];
  esp_wifi_get_mac(WIFI_IF_STA, mac);

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  Serial.print("MAC deste ESP32: ");
  Serial.println(macStr);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Pronto para receber");
}

void loop() {


  
}










