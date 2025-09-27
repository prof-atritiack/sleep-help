# 📡 Protocolo de Comunicação ESP32 → Sleep Help

## 🎯 Visão Geral

Este documento descreve como o ESP32 deve enviar dados de sensores (SpO2 e BPM) para a aplicação Sleep Help.

## 🔌 Configuração da Rede

### IP do Servidor
- **Desenvolvimento**: `http://10.10.14.223:3001` (IP da sua máquina na rede)
- **Produção**: `http://[IP_DO_SERVIDOR]:3001`

### Endpoint Principal
```
POST http://10.10.14.223:3001/api/sensor-data
```

## 📊 Formato dos Dados

### Estrutura JSON
```json
{
  "spo2": 98,
  "bpm": 72,
  "timestamp": "2025-09-27 10:30:45"
}
```

### Campos Obrigatórios
- **`spo2`** (integer): Saturação de oxigênio (0-100%)
- **`bpm`** (integer): Batimentos por minuto (30-200)
- **`timestamp`** (string, opcional): Data/hora no formato "YYYY-MM-DD HH:mm:ss"

## 🔧 Código ESP32 (Arduino)

### Bibliotecas Necessárias
```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
```

### Configuração Wi-Fi
```cpp
const char* ssid = "academico.street.br";  // Nome da sua rede
const char* password = "sua_senha";        // Senha da rede
const char* serverURL = "http://10.10.14.223:3001/api/sensor-data";
```

### Função para Enviar Dados
```cpp
void sendSensorData(int spo2, int bpm) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");
    
    // Criar JSON
    String jsonData = "{";
    jsonData += "\"spo2\":" + String(spo2) + ",";
    jsonData += "\"bpm\":" + String(bpm) + ",";
    jsonData += "\"timestamp\":\"" + getCurrentTime() + "\"";
    jsonData += "}";
    
    // Enviar POST
    int httpResponseCode = http.POST(jsonData);
    
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
```

### Função para Obter Timestamp
```cpp
String getCurrentTime() {
  // Implementar RTC ou usar NTP
  // Exemplo simples:
  return "2025-09-27 10:30:45";
}
```

### Loop Principal
```cpp
void loop() {
  // Ler sensores
  int spo2Value = readSpO2Sensor();
  int bpmValue = readBPMSensor();
  
  // Enviar dados a cada 5 segundos
  static unsigned long lastSend = 0;
  if (millis() - lastSend >= 5000) {
    sendSensorData(spo2Value, bpmValue);
    lastSend = millis();
  }
  
  delay(100);
}
```

## 📡 Exemplo Completo ESP32

```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Configurações de rede
const char* ssid = "academico.street.br";
const char* password = "sua_senha";
const char* serverURL = "http://10.10.14.223:3001/api/sensor-data";

// Pinos dos sensores (ajustar conforme hardware)
#define SPO2_PIN A0
#define BPM_PIN A1

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
}

void loop() {
  // Simular leitura de sensores
  int spo2 = analogRead(SPO2_PIN);
  int bpm = analogRead(BPM_PIN);
  
  // Converter valores analógicos para valores reais
  spo2 = map(spo2, 0, 4095, 90, 100);  // 90-100%
  bpm = map(bpm, 0, 4095, 60, 100);    // 60-100 bpm
  
  // Enviar dados
  sendSensorData(spo2, bpm);
  
  delay(5000); // Enviar a cada 5 segundos
}

void sendSensorData(int spo2, int bpm) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");
    
    // Criar JSON
    DynamicJsonDocument doc(1024);
    doc["spo2"] = spo2;
    doc["bpm"] = bpm;
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
  }
}

String getCurrentTime() {
  // Implementar com RTC ou NTP
  return "2025-09-27 10:30:45";
}
```

## 🔍 Testando a Comunicação

### 1. Teste com cURL
```bash
curl -X POST http://10.10.14.223:3001/api/sensor-data \
  -H "Content-Type: application/json" \
  -d '{"spo2":98,"bpm":72,"timestamp":"2025-09-27 10:30:45"}'
```

### 2. Teste com Postman
- **Método**: POST
- **URL**: `http://10.10.14.223:3001/api/sensor-data`
- **Headers**: `Content-Type: application/json`
- **Body**:
```json
{
  "spo2": 98,
  "bpm": 72,
  "timestamp": "2025-09-27 10:30:45"
}
```

## 📊 Respostas da API

### Sucesso (200)
```json
{
  "success": true,
  "message": "Dados recebidos com sucesso",
  "data": {
    "spo2": 98,
    "bpm": 72,
    "timestamp": "2025-09-27 10:30:45",
    "unix": 1695812445000
  }
}
```

### Erro (400)
```json
{
  "error": "Dados inválidos. SpO2 e BPM são obrigatórios."
}
```

## 🔧 Endpoints Disponíveis

### 1. Enviar Dados do Sensor
```
POST /api/sensor-data
```

### 2. Obter Dados Atuais
```
GET /api/current-data
```

### 3. Obter Dados Históricos
```
GET /api/sensor-data?limit=100
```

### 4. Status do Servidor
```
GET /api/status
```

## 🚨 Validações

### SpO2
- **Faixa válida**: 0-100%
- **Alerta**: < 95% (baixa saturação)

### BPM
- **Faixa válida**: 30-200 bpm
- **Normal**: 60-100 bpm
- **Alerta**: < 60 ou > 100 bpm

## 🔄 Comunicação em Tempo Real

A aplicação usa **WebSocket** para atualizações em tempo real:

- **URL**: `ws://10.10.14.223:3001`
- **Evento**: `sensorData`
- **Dados**: Mesmo formato JSON do POST

## 🛠️ Troubleshooting

### Problemas Comuns

1. **Wi-Fi não conecta**
   - Verificar SSID e senha
   - Verificar sinal da rede

2. **HTTP 404**
   - Verificar URL do servidor
   - Verificar se o backend está rodando

3. **HTTP 500**
   - Verificar formato JSON
   - Verificar se os campos obrigatórios estão presentes

4. **Timeout**
   - Verificar conectividade de rede
   - Aumentar timeout do HTTPClient

### Logs Úteis

```cpp
// No ESP32
Serial.println("Wi-Fi Status: " + String(WiFi.status()));
Serial.println("IP: " + WiFi.localIP().toString());
Serial.println("RSSI: " + String(WiFi.RSSI()));

// No backend (console)
📊 Dados recebidos: SpO2=98%, BPM=72
🔌 Cliente conectado via WebSocket
```

## 📱 Monitoramento

A aplicação web mostrará:
- ✅ **Status de conexão** com o ESP32
- 📊 **Dados em tempo real** (SpO2 e BPM)
- 🚨 **Alertas automáticos** para valores anormais
- 📈 **Histórico de medições**

---

**Desenvolvido para o TCC em Automação Industrial - ETEC Jorge Street**
