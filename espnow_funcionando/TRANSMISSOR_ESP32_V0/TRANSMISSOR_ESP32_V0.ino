#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

// MAC do receptor (já corrigido)
uint8_t receiverAddress[] = {0xF4, 0x65, 0x0B, 0x57, 0xBF, 0xF8};

typedef struct {
  int id;
  bool ledOn;
} mensagem;

mensagem dados;

void setup() {
  Serial.begin(115200);

  // Ativa WiFi modo estação
  WiFi.mode(WIFI_STA);
  esp_wifi_start();

  Serial.println("SLEEP HELP - 3CT");
  delay(2000);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  // Registra o receptor como peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Erro ao adicionar peer");
    return;
  }

  Serial.println("Pronto pra enviar");

  // Define ID fixo do transmissor
  dados.id = 1;
}

void loop() {
  // Gera sinal aleatório 0/1
  dados.ledOn = random(0, 2);

  // Envia dados para o receptor
  esp_now_send(receiverAddress, (uint8_t *)&dados, sizeof(dados));

  Serial.print("Enviado -> ID: ");
  Serial.print(dados.id);
  Serial.print(" | LED: ");
  Serial.println(dados.ledOn ? "ON" : "OFF");

  delay(2000);
}
