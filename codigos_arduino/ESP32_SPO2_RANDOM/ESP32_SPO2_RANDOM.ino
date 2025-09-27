#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Configurações de rede
const char* ssid = "academico.street.br";  // Nome da sua rede Wi-Fi
const char* password = "sua_senha";        // Senha da sua rede Wi-Fi
const char* serverURL = "http://172.20.10.7:3001/api/sensor-data";

// Configurações do sensor
#define LED_PIN 2  // LED interno do ESP32
#define INTERVALO_ENVIO 5000  // Intervalo entre envios (5 segundos)

// Variáveis
unsigned long ultimoEnvio = 0;
int contador = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("========================================");
  Serial.println("    SLEEP HELP - ESP32 SpO2 Random");
  Serial.println("    Enviando dados aleatórios para API");
  Serial.println("========================================");
  
  // Conectar ao Wi-Fi
  conectarWiFi();
  
  Serial.println("Sistema iniciado!");
  Serial.println("Enviando dados para: " + String(serverURL));
  Serial.println("Intervalo: " + String(INTERVALO_ENVIO/1000) + " segundos");
  Serial.println("========================================");
}

void loop() {
  // Verificar conexão Wi-Fi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ Wi-Fi desconectado! Tentando reconectar...");
    conectarWiFi();
    return;
  }
  
  // Enviar dados no intervalo configurado
  if (millis() - ultimoEnvio >= INTERVALO_ENVIO) {
    enviarDadosSpO2();
    ultimoEnvio = millis();
  }
  
  delay(100);
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

void enviarDadosSpO2() {
  // Gerar valor aleatório de SpO2 (90-100%)
  int spo2 = random(90, 101);
  
  // Gerar timestamp atual
  String timestamp = getCurrentTime();
  
  // Incrementar contador
  contador++;
  
  Serial.println("📊 Enviando dados #" + String(contador));
  Serial.println("   SpO2: " + String(spo2) + "%");
  Serial.println("   Timestamp: " + timestamp);
  
  // Piscar LED durante envio
  digitalWrite(LED_PIN, HIGH);
  
  // Enviar dados via HTTP POST
  if (enviarHTTP(spo2, timestamp)) {
    Serial.println("✅ Dados enviados com sucesso!");
    
    // Verificar se é valor de alerta
    if (spo2 < 95) {
      Serial.println("⚠️  ALERTA: Baixa saturação detectada!");
      piscarLED(3, 200);  // Piscar 3 vezes para alerta
    } else {
      Serial.println("✅ Saturação normal");
      piscarLED(1, 100);  // Piscar 1 vez para normal
    }
  } else {
    Serial.println("❌ Falha ao enviar dados!");
    piscarLED(5, 100);  // Piscar 5 vezes para erro
  }
  
  digitalWrite(LED_PIN, LOW);
  Serial.println("----------------------------------------");
}

bool enviarHTTP(int spo2, String timestamp) {
  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }
  
  HTTPClient http;
  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");
  
  // Criar JSON
  DynamicJsonDocument doc(1024);
  doc["spo2"] = spo2;
  doc["timestamp"] = timestamp;
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  // Enviar POST
  int httpResponseCode = http.POST(jsonString);
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("   Resposta HTTP: " + String(httpResponseCode));
    Serial.println("   Resposta: " + response);
    http.end();
    return httpResponseCode == 200;
  } else {
    Serial.println("   Erro HTTP: " + String(httpResponseCode));
    http.end();
    return false;
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

// Função para testar conexão com a API
void testarAPI() {
  Serial.println("🔍 Testando conexão com API...");
  
  HTTPClient http;
  http.begin("http://172.20.10.7:3001/api/status");
  
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
