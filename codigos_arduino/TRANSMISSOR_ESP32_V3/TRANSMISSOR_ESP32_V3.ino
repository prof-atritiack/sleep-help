#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <WiFi.h>
#include <esp_now.h>
#include "tft_setup.h"          // Mantenha exatamente esta linha para evitar erros
#include <TFT_eSPI.h>

// --- CONFIGURAÇÕES ---
#define BUFFER_SIZE 100
#define BUZZER_PIN 4
#define LED_PIN 2
#define DEDO_THRESHOLD 30000

// MAC do receptor ESP32
uint8_t broadcastAddress[] = {0xF4, 0x65, 0x0B, 0x57, 0xBF, 0xF8};

// Struct idêntica à do receptor para ESP-NOW
typedef struct {
  int id;
  bool ledOn;
} mensagem;
mensagem dados;

MAX30105 particleSensor;
TFT_eSPI tft = TFT_eSPI();

// Callback simplificada para evitar erro de acesso inválido a membros da struct
void onSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  Serial.print("Status do envio: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sucesso" : "Falha");
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(2);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.setCursor(10, 10);
  tft.print("Oximetro ESP32");

  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("Sensor MAX30102 nao encontrado");
    tft.setCursor(10, 40);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("Sensor Nao Encontrado");
    while (true);
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeIR(0x1F);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW");
    tft.setCursor(10, 70);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("Erro ESP-NOW");
    while (true);
  }

  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Erro ao adicionar peer");
    tft.setCursor(10, 100);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("Erro Peer");
    while (true);
  }
}

void loop() {
  long irSum = 0;
  int nTest = 5;
  for (int i = 0; i < nTest; i++) {
    while (!particleSensor.available()) particleSensor.check();
    irSum += particleSensor.getIR();
    delay(5);
  }
  long irAvg = irSum / nTest;
  Serial.print("Valor medio IR: ");
  Serial.println(irAvg);

  tft.fillRect(0, 40, tft.width(), 40, TFT_BLACK);

  if (irAvg > DEDO_THRESHOLD) {
    uint32_t irBuffer[BUFFER_SIZE];
    uint32_t redBuffer[BUFFER_SIZE];

    for (int i = 0; i < BUFFER_SIZE; i++) {
      while (!particleSensor.available()) particleSensor.check();
      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample();
    }

    int32_t spo2;
    int8_t valid_spo2;
    int32_t heartRate;
    int8_t valid_hr;

    maxim_heart_rate_and_oxygen_saturation(irBuffer, BUFFER_SIZE, redBuffer,
                                           &spo2, &valid_spo2, &heartRate, &valid_hr);

    if (valid_spo2 && valid_hr) {
      Serial.print("SpO2: ");
      Serial.print(spo2);
      Serial.print("%  HR: ");
      Serial.print(heartRate);
      Serial.println(" bpm");

      tft.setCursor(10, 45);
      tft.setTextColor(TFT_PINK, TFT_BLACK);
      tft.print("SpO2: ");
      tft.print(spo2);
      tft.print(" %");

      tft.setCursor(10, 80);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.print("BPM: ");
      tft.print(heartRate);

      dados.id = 1;
      dados.ledOn = (spo2 > 90);

      digitalWrite(BUZZER_PIN, dados.ledOn);
      digitalWrite(LED_PIN, dados.ledOn);

    } else {
      Serial.println("Medicao invalida");
      dados.id = 1;
      dados.ledOn = false;
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
    }

    esp_now_send(broadcastAddress, (uint8_t *)&dados, sizeof(dados));
    
  } else {
    Serial.println("Coloque o dedo no sensor");
    dados.id = 1;
    dados.ledOn = false;
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);

    tft.setCursor(10, 35);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("Coloque o dedo");

    tft.setCursor(10, 170);
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.print("SLEEP HELP");

    tft.fillRect(0, 65, tft.width(), 35, TFT_BLACK);

    esp_now_send(broadcastAddress, (uint8_t *)&dados, sizeof(dados));
  }
  delay(3000);
}