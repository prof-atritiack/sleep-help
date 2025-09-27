#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <esp_now.h>
#include <esp_wifi.h>

// Configurações de rede
const char* ssid = "academico.street.br";  // Nome da sua rede
const char* password = "sua_senha";        // Senha da rede
const char* serverURL = "http://172.20.10.7:3001/api/sensor-data";

#define LED 13

// Estrutura para dados do sensor via ESP-NOW
typedef struct {
  int spo2;
  String timestamp;
} sensorData;

sensorData dados;

// Callback ESP-NOW (core 3.x)
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&dados, incomingData, sizeof(dados));

  Serial.print("📊 Dados recebidos via ESP-NOW: ");
  Serial.print("SpO2: ");
  Serial.print(dados.spo2);
  Serial.print("% | Timestamp: ");
  Serial.println(dados.timestamp);

  // Enviar dados para a aplicação web
  sendToWebApp(dados.spo2);
  
  // Controlar LED baseado na saturação
  if (dados.spo2 < 95) {
    digitalWrite(LED, HIGH);  // LED aceso para alerta
    Serial.println("⚠️ ALERTA: Baixa saturação detectada!");
  } else {
    digitalWrite(LED, LOW);   // LED apagado para normal
    Serial.println("✅ Saturação normal");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  Serial.println("SLEEP HELP - Receptor SpO2 Gateway");
  delay(2000);

  // Conectar Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Wi-Fi conectado!");
  Serial.println("IP: " + WiFi.localIP().toString());

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

  Serial.println("Pronto para receber dados SpO2 via ESP-NOW");
  Serial.println("Enviando para: " + String(serverURL));
}

void loop() {
  // Loop vazio - tudo é feito via callback
  delay(100);
}

void sendToWebApp(int spo2) {
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
      Serial.println("✅ Dados enviados para web: " + response);
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
