#include <Wire.h>
#include <MAX30105.h>
#include <spo2_algorithm.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <tft_setup.h>      // Mantenha exatamente esta linha para evitar erros
#include <TFT_eSPI.h>

// --- CONFIGURAÇÕES ---
#define BUFFER_SIZE 100
#define BUZZER_PIN 4
#define LED_PIN 2
#define DEDO_THRESHOLD 30000
#define INTERVALO_ENVIO 5000  // Intervalo entre envios para API (5 segundos)

// Configurações de rede
const char* ssid = "iPhone de André";    // Nome da sua rede Wi-Fi
const char* password = "bellabella1302";        // Senha da sua rede Wi-Fi
const char* serverIP = "172.20.10.7"; // Variável Separada para o IP
const char* apiPort = "3001";
const String serverURL = "http://" + String(serverIP) + ":" + String(apiPort) + "/api/sensor-data";

// Variáveis para controle de tempo
unsigned long ultimoEnvioAPI = 0;
int contadorMedicoes = 0;

MAX30105 particleSensor;
TFT_eSPI tft = TFT_eSPI();

// --- FUNÇÕES DE SETUP E UTILIADE (AUXILIARES) ---

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

void enviarParaAPI(int spo2) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ Wi-Fi desconectado! Tentando reconectar...");
    conectarWiFi();
    return;
  }
  
  Serial.println("📊 Enviando dados para API...");
  Serial.println("   SpO2: " + String(spo2) + "%");
  Serial.println("   Timestamp: " + getCurrentTime());
  
  // Usando String(serverURL) para garantir que o tipo seja const char* para http.begin
  HTTPClient http;
  http.begin(serverURL.c_str()); 
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
    
    // Lógica de Alerta
    if (spo2 < 95) {
      Serial.println("⚠️ ALERTA: Baixa saturação detectada!");
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

void testarAPI() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Não é possível testar a API: Wi-Fi desconectado.");
    return;
  }
  
  Serial.println("🔍 Testando conexão com API...");
  
  String statusURL = "http://" + String(serverIP) + ":" + String(apiPort) + "/api/status";
  HTTPClient http;
  http.begin(statusURL.c_str());
  
  int httpResponseCode = http.GET();
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("✅ API respondendo!");
    Serial.println("   Status: " + String(httpResponseCode));
    Serial.println("   Resposta: " + response);
  } else {
    Serial.println("❌ API não respondeu!");
    Serial.println("   Erro: " + String(httpResponseCode));
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
    Serial.println("Sensor MAX30102 nao encontrado");
    tft.setCursor(10, 40);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("Sensor Nao Encontrado");
    while (true);
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeIR(0x1F);

  // Conectar ao Wi-Fi
  conectarWiFi();

  Serial.println("========================================");
  Serial.println("      SLEEP HELP - ESP32 Oximetro");
  Serial.println("        Sensor MAX30105 + API");
  Serial.println("========================================");
  Serial.println("Enviando dados para: " + serverURL);
  Serial.println("========================================");
}

// --- LOOP ---
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

  tft.fillRect(0, 40, tft.width(), 40, TFT_BLACK); // Limpa área de SpO2

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
    // Removendo heartRate e valid_hr
    int32_t heartRate; // Mantido para a função maxim_heart_rate_and_oxygen_saturation, mas seu valor não é usado
    int8_t valid_hr;

    // A função de algoritmo ainda requer 4 ponteiros, mas usaremos apenas SpO2
    maxim_heart_rate_and_oxygen_saturation(irBuffer, BUFFER_SIZE, redBuffer,
                                           &spo2, &valid_spo2, &heartRate, &valid_hr); 
    // Nota: O cálculo de BPM (heartRate) ainda é executado, mas é descartado/ignorado.

    if (valid_spo2) { // Não checa mais valid_hr
      contadorMedicoes++;
      Serial.print("Medicao #");
      Serial.print(contadorMedicoes);
      Serial.print(" - SpO2: ");
      Serial.print(spo2);
      Serial.println("%");
      // Serial.println(" bpm"); // Removido

      // Atualizar display
      tft.setCursor(10, 45);
      tft.setTextColor(TFT_PINK, TFT_BLACK);
      tft.print("SpO2: ");
      tft.print(spo2);
      tft.print(" %");

      // tft.setCursor(10, 80); // Linha removida
      // tft.print("BPM: "); // Linha removida
      // tft.print(heartRate); // Linha removida
      tft.fillRect(0, 80, tft.width(), 30, TFT_BLACK); // Limpa a antiga área de BPM

      // Lógica de alerta
      bool alerta = (spo2 <= 95);
      digitalWrite(BUZZER_PIN, alerta);
      digitalWrite(LED_PIN, alerta);

      // Mostrar status da API
      tft.setCursor(10, 115);
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.print("API: Conectado");

      // Enviar para API se passou o intervalo
      if (millis() - ultimoEnvioAPI >= INTERVALO_ENVIO) {
        enviarParaAPI(spo2);
        ultimoEnvioAPI = millis();
      }

    } else {
      Serial.println("Medicao invalida");
      
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(LED_PIN, LOW);

      tft.setCursor(10, 45);
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.print("Medicao Invalida");
      
      tft.fillRect(0, 80, tft.width(), 30, TFT_BLACK); // Limpa a área
    }
    
  } else {
    Serial.println("Coloque o dedo no sensor");
    
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);

    tft.setCursor(10, 35);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("Coloque o dedo");

    tft.setCursor(10, 170);
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.print("SLEEP HELP");

    tft.fillRect(0, 65, tft.width(), 35, TFT_BLACK); // Limpa área de BPM

    // Mostrar status da API
    tft.setCursor(10, 115);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.print("API: Aguardando");
  }
  
  delay(3000);
}