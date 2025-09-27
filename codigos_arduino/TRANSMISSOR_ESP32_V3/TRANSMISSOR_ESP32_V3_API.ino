#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <WiFi.h>
#include <esp_now.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "tft_setup.h"          // Mantenha exatamente esta linha para evitar erros
#include <TFT_eSPI.h>

// --- CONFIGURAÇÕES ---
#define BUFFER_SIZE 100
#define BUZZER_PIN 4
#define LED_PIN 2
#define DEDO_THRESHOLD 30000
#define INTERVALO_ENVIO_API 5000  // Intervalo entre envios para API (5 segundos)

// Configurações de rede para API
const char* ssid = "academico.street.br";  // Nome da sua rede Wi-Fi
const char* password = "sua_senha";        // Senha da sua rede Wi-Fi
const char* serverURL = "http://172.20.10.7:3001/api/sensor-data";

// MAC do receptor ESP32
uint8_t broadcastAddress[] = {0xF4, 0x65, 0x0B, 0x57, 0xBF, 0xF8};

// Struct idêntica à do receptor para ESP-NOW
typedef struct {
  int id;
  bool ledOn;
} mensagem;
mensagem dados;

// Variáveis para controle de tempo da API
unsigned long ultimoEnvioAPI = 0;
int contadorMedicoes = 0;

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

  // Conectar ao Wi-Fi para API
  conectarWiFi();

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

  Serial.println("========================================");
  Serial.println("    SLEEP HELP - ESP32 Oximetro");
  Serial.println("    Sensor MAX30105 + ESP-NOW + API");
  Serial.println("========================================");
  Serial.println("Enviando dados para: " + String(serverURL));
  Serial.println("========================================");
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

    if (valid_spo2) {
      contadorMedicoes++;
      Serial.print("Medicao #");
      Serial.print(contadorMedicoes);
      Serial.print(" - SpO2: ");
      Serial.print(spo2);
      Serial.println("%");

      tft.setCursor(10, 45);
      tft.setTextColor(TFT_PINK, TFT_BLACK);
      tft.print("SpO2: ");
      tft.print(spo2);
      tft.print(" %");


      // Mostrar status da API
      tft.setCursor(10, 115);
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.print("API: Conectado");

      dados.id = 1;
      dados.ledOn = (spo2 > 90);

      digitalWrite(BUZZER_PIN, dados.ledOn);
      digitalWrite(LED_PIN, dados.ledOn);

      // Enviar para API se passou o intervalo
      if (millis() - ultimoEnvioAPI >= INTERVALO_ENVIO_API) {
        enviarParaAPI(spo2);
        ultimoEnvioAPI = millis();
      }

    } else {
      Serial.println("Medicao invalida");
      dados.id = 1;
      dados.ledOn = false;
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(LED_PIN, LOW);

      tft.setCursor(10, 45);
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.print("Medicao Invalida");
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

    // Mostrar status da API
    tft.setCursor(10, 115);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.print("API: Aguardando");

    esp_now_send(broadcastAddress, (uint8_t *)&dados, sizeof(dados));
  }
  delay(3000);
}

void conectarWiFi() {
  WiFi.begin(ssid, password);
  
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(1000);
    Serial.print(".");
    tentativas++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("✅ Wi-Fi conectado!");
    Serial.println("IP: " + WiFi.localIP().toString());
    Serial.println("RSSI: " + String(WiFi.RSSI()) + " dBm");
  } else {
    Serial.println();
    Serial.println("❌ Falha ao conectar Wi-Fi!");
    Serial.println("Verifique as credenciais e tente novamente.");
  }
}

void enviarParaAPI(int spo2) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ Wi-Fi desconectado! Tentando reconectar...");
    conectarWiFi();
    return;
  }
  
  Serial.println("📊 Enviando dados para API...");
  Serial.println("   SpO2: " + String(spo2) + "%");
  Serial.println("   Timestamp: " + getCurrentTime());
  
  HTTPClient http;
  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");
  
  // Criar JSON
  DynamicJsonDocument doc(1024);
  doc["spo2"] = spo2;
  doc["timestamp"] = getCurrentTime();
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  // Enviar POST
  int httpResponseCode = http.POST(jsonString);
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("✅ Dados enviados para API: " + response);
    
    // Verificar se é valor de alerta
    if (spo2 < 95) {
      Serial.println("⚠️  ALERTA: Baixa saturação detectada!");
      piscarLED(3, 200);  // Piscar 3 vezes para alerta
    } else {
      Serial.println("✅ Saturação normal");
      piscarLED(1, 100);  // Piscar 1 vez para normal
    }
  } else {
    Serial.println("❌ Falha ao enviar para API: " + String(httpResponseCode));
    piscarLED(5, 100);  // Piscar 5 vezes para erro
  }
  
  http.end();
  Serial.println("----------------------------------------");
}

String getCurrentTime() {
  // Simular timestamp (em produção, usar RTC ou NTP)
  unsigned long tempo = millis() / 1000;
  int segundos = tempo % 60;
  int minutos = (tempo / 60) % 60;
  int horas = (tempo / 3600) % 24;
  
  char timestamp[20];
  sprintf(timestamp, "2025-09-27 %02d:%02d:%02d", horas, minutos, segundos);
  return String(timestamp);
}

void piscarLED(int vezes, int delay_ms) {
  for (int i = 0; i < vezes; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(delay_ms);
    digitalWrite(LED_PIN, LOW);
    delay(delay_ms);
  }
}