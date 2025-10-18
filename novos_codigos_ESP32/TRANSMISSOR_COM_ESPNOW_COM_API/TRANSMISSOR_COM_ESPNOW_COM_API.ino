#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "tft_setup.h"        // Mantenha exatamente esta linha para evitar erros
#include <TFT_eSPI.h>
#include <esp_now.h>
#include <esp_wifi.h>

// --- CONFIGURAÇÕES DE HARDWARE E SENSORES ---
#define BUFFER_SIZE 100
#define BUZZER_PIN 4
#define LED_PIN 2
#define DEDO_THRESHOLD 30000

// --- CONFIGURAÇÕES DE COMUNICAÇÃO ---
#define INTERVALO_ENVIO_NOW 3000   // Intervalo de leitura (e envio NOW)
#define INTERVALO_ENVIO_API 30000  // Intervalo para envio à API (30 segundos)

// Configurações de rede
const char* ssid = "iPhone de André";    
const char* password = "bellabella1302";       
const char* serverIP = "172.20.10.7"; 
const char* apiPort = "3001";
const String serverURL = "http://" + String(serverIP) + ":" + String(apiPort) + "/api/sensor-data";

// MAC do receptor ESP32 (para ESP-NOW)
uint8_t broadcastAddress[] = {0xF4, 0x65, 0x0B, 0x57, 0xBF, 0xF8};

// Struct para ESP-NOW
typedef struct {
  int id;
  bool ledOn; // True se houver alerta (SpO2 <= 95)
} mensagem;
mensagem dados;

// Variáveis para controle de tempo
unsigned long ultimoEnvioAPI = 0;
unsigned long ultimoEnvioNOW = 0;
int contadorMedicoes = 0;

MAX30105 particleSensor;
TFT_eSPI tft = TFT_eSPI();


// --- FUNÇÕES DE CALLBACKS E UTILIADE ---

// Callback corrigida para o erro de compilação
void onSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  Serial.print("[NOW] Status Envio: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "✅ Sucesso" : "❌ Falha");
}

void conectarWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int tentativas = 0;
  Serial.print("\n[WIFI] Conectando");
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(1000);
    Serial.print(".");
    tentativas++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("[WIFI] ✅ Wi-Fi conectado!");
    Serial.println("[WIFI] IP: " + WiFi.localIP().toString());
  } else {
    Serial.println();
    Serial.println("[WIFI] ❌ Falha ao conectar Wi-Fi!");
  }
}

String getCurrentTime() {
  unsigned long tempo = millis() / 1000;
  int segundos = tempo % 60;
  int minutos = (tempo / 60) % 60;
  int horas = (tempo / 3600) % 24;
  
  char timestamp[20];
  sprintf(timestamp, "%02d:%02d:%02d", horas, minutos, segundos);
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

void enviarParaAPI(int spo2) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[API] ❌ Wi-Fi desconectado! Tentando reconectar...");
    conectarWiFi();
    return;
  }
  
  Serial.println("\n[API] 📊 Enviando dados...");
  Serial.println("      SpO2: " + String(spo2) + "%");
  Serial.println("      Timestamp: " + getCurrentTime());
  
  HTTPClient http;
  http.begin(serverURL.c_str()); 
  http.addHeader("Content-Type", "application/json");
  
  DynamicJsonDocument doc(1024);
  doc["spo2"] = spo2;
  doc["timestamp"] = getCurrentTime();
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  int httpResponseCode = http.POST(jsonString);
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("[API] ✅ Envio Sucesso. Código: " + String(httpResponseCode));
    
    // Lógica de piscar LED APÓS envio para sinalizar status da API
    if (spo2 < 95) {
      Serial.println("[API] ⚠️ ALERTA: Baixa saturação detectada!");
      piscarLED(3, 200);
    } else {
      Serial.println("[API] ✅ Saturação normal.");
      piscarLED(1, 100);
    }
  } else {
    Serial.println("[API] ❌ Falha no envio. Código: " + String(httpResponseCode));
    piscarLED(5, 100);
  }
  
  http.end();
}


// --- SETUP ---
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Inicializar TFT
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(2);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.setCursor(10, 10);
  tft.print("Oximetro ESP32");

  // Inicializar sensor MAX30105
  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("[SENSOR] Sensor MAX30102 nao encontrado");
    tft.setCursor(10, 40);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("Sensor Nao Encontrado");
    while (true);
  }
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeIR(0x1F);

  // --- CONFIGURAÇÃO DE COMUNICAÇÃO ---
  conectarWiFi(); // Inicializa Wi-Fi (modo STA)

  // Configurar ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("[NOW] ❌ Erro ao iniciar ESP-NOW");
    tft.setCursor(10, 70);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("Erro ESP-NOW");
    while (true);
  }

  esp_now_register_send_cb(onSent);

  // Adicionar Peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[NOW] ❌ Erro ao adicionar peer");
    tft.setCursor(10, 100);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("Erro Peer");
    while (true);
  }

  // Define ID fixo e inicializa dados para NOW
  dados.id = 1;
  dados.ledOn = false; 

  Serial.println("========================================");
  Serial.println("  SLEEP HELP - ESP32 Oximetro (Sender)");
  Serial.println("  NOW -> Alarme | API -> Dados Completos");
  Serial.println("========================================");
}

// --- LOOP ---
void loop() {
  
  if (millis() - ultimoEnvioNOW >= INTERVALO_ENVIO_NOW) {
    
    // 1. LEITURA DO SENSOR
    long irSum = 0;
    int nTest = 5;
    for (int i = 0; i < nTest; i++) {
      while (!particleSensor.available()) particleSensor.check();
      irSum += particleSensor.getIR();
      delay(5);
    }
    long irAvg = irSum / nTest;
    Serial.print("\n[SENSOR] IR Medio: ");
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
        Serial.print("[MEDICAO] #");
        Serial.print(contadorMedicoes);
        Serial.print(" | SpO2: ");
        Serial.print(spo2);
        Serial.println("%");

        // 2. LÓGICA DE ALERTA (Local e ESP-NOW)
        bool alerta = (spo2 <= 95);
        
        // CONTROLE LOCAL: Usa a lógica de alerta do SPO2 diretamente (lógica original)
        digitalWrite(BUZZER_PIN, alerta);
        digitalWrite(LED_PIN, alerta); // O LED piscará apenas no envio API, aqui fica ligado/desligado

        // CONFIGURA DADOS PARA ESP-NOW: SINALIZA O ALERTA
        dados.ledOn = alerta; 

        // ATUALIZA TFT
        tft.setCursor(10, 45);
        tft.setTextColor(TFT_PINK, TFT_BLACK);
        tft.print("SpO2: ");
        tft.print(spo2);
        tft.print(" %");
        tft.fillRect(0, 80, tft.width(), 30, TFT_BLACK);
        tft.setCursor(10, 115);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.print("API: Pronto");

        // 3. ENVIO ESP-NOW (imediato)
        esp_now_send(broadcastAddress, (uint8_t *)&dados, sizeof(dados));
        Serial.print("[NOW] Enviando Alarme: ");
        Serial.println(dados.ledOn ? "ON" : "OFF");

        // 4. ENVIO API (temporizado: 30 segundos)
        if (millis() - ultimoEnvioAPI >= INTERVALO_ENVIO_API) {
          enviarParaAPI(spo2);
          ultimoEnvioAPI = millis();
        }

      } else {
        Serial.println("[MEDICAO] ⚠️ Medicao invalida. Aguardando...");
        dados.ledOn = false;
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(LED_PIN, LOW);

        tft.setCursor(10, 45);
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.print("Medicao Invalida");
        tft.fillRect(0, 80, tft.width(), 30, TFT_BLACK);
      }
      
    } else {
      // Dedo fora do sensor
      Serial.println("[SENSOR] 🛑 Coloque o dedo no sensor");
      
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

      tft.setCursor(10, 115);
      tft.setTextColor(TFT_ORANGE, TFT_BLACK);
      tft.print("API: Aguardando");

      // Envia status 'desligado' via ESP-NOW
      esp_now_send(broadcastAddress, (uint8_t *)&dados, sizeof(dados));
    }
    
    ultimoEnvioNOW = millis();
  }
}