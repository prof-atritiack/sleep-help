#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Configurações de rede
const char* ssid = "academico.street.br";  // Nome da sua rede
const char* password = "sua_senha";        // Senha da rede
const char* serverURL = "http://172.20.10.7:3001/api/sensor-data";

// Pinos dos sensores (ajustar conforme hardware)
#define SPO2_PIN A0

// Estrutura para dados do sensor
typedef struct {
  int spo2;
  String timestamp;
} sensorData;

sensorData dados;

void setup() {
  Serial.begin(115200);
  
  // Conectar Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Wi-Fi conectado!");
  Serial.println("IP: " + WiFi.localIP().toString());
  
  Serial.println("SLEEP HELP - Transmissor SpO2");
  Serial.println("Enviando dados para: " + String(serverURL));
}

void loop() {
  // Simular leitura de sensor SpO2
  int spo2 = analogRead(SPO2_PIN);
  
  // Converter valores analógicos para valores reais
  spo2 = map(spo2, 0, 4095, 90, 100);  // 90-100%
  
  // Criar dados do sensor
  dados.spo2 = spo2;
  dados.timestamp = getCurrentTime();
  
  // Enviar dados para a aplicação web
  sendSensorData(dados.spo2);
  
  Serial.println("SpO2: " + String(dados.spo2) + "% | Timestamp: " + dados.timestamp);
  
  delay(5000); // Enviar a cada 5 segundos
}

void sendSensorData(int spo2) {
  if (WiFi.status() == WL_CONNECTED) {
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
      Serial.println("✅ Dados enviados: " + response);
    } else {
      Serial.println("❌ Erro HTTP: " + String(httpResponseCode));
    }
    
    http.end();
  } else {
    Serial.println("❌ Wi-Fi desconectado");
  }
}

String getCurrentTime() {
  // Implementar com RTC ou NTP para timestamp real
  // Por enquanto, retorna timestamp simulado
  return "2025-09-27 12:00:00";
}
