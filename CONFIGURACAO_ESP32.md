# 🔧 Configuração ESP32 para Sleep Help

## 📋 **Pré-requisitos**

### **Hardware Necessário:**
- 2x ESP32 DevKit V1
- Sensor de SpO2 (ex: MAX30100)
- Cabos de conexão
- Protoboard (opcional)

### **Software:**
- Arduino IDE
- Biblioteca ESP32 (versão 2.0+)
- Bibliotecas: WiFi, HTTPClient, ArduinoJson

## 🚀 **Passo a Passo**

### **1. Configurar o Receptor (Gateway)**

1. **Abra o Arduino IDE**
2. **Carregue o código**: `RECEIVER_ESPNOW_V0_SPO2.ino`
3. **Configure as credenciais Wi-Fi**:
   ```cpp
   const char* ssid = "academico.street.br";
   const char* password = "sua_senha";
   ```
4. **Configure o IP do servidor**:
   ```cpp
   const char* serverURL = "http://10.10.14.223:3001/api/sensor-data";
   ```
5. **Faça upload para o ESP32 receptor**

### **2. Configurar o Transmissor (Sensor)**

1. **Abra o Arduino IDE**
2. **Carregue o código**: `TRANSMISSOR_ESP32_V0_SPO2.ino`
3. **Configure as mesmas credenciais Wi-Fi**
4. **Configure o mesmo IP do servidor**
5. **Conecte o sensor de SpO2 no pino A0**
6. **Faça upload para o ESP32 transmissor**

### **3. Configurar Comunicação ESP-NOW**

1. **Anote o MAC do receptor** (aparece no Serial Monitor)
2. **No código do transmissor, atualize o MAC**:
   ```cpp
   uint8_t receiverAddress[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};
   ```

## 🔌 **Conexões do Hardware**

### **Sensor MAX30100:**
```
VCC  → 3.3V
GND  → GND
SDA  → GPIO 21
SCL  → GPIO 22
```

### **LED de Status:**
```
LED  → GPIO 13
GND  → GND
```

## 📊 **Testando o Sistema**

### **1. Verificar Receptor:**
- Abra o Serial Monitor (115200 baud)
- Deve mostrar: "Pronto para receber dados SpO2 via ESP-NOW"
- Anote o MAC address exibido

### **2. Verificar Transmissor:**
- Abra o Serial Monitor (115200 baud)
- Deve mostrar: "Wi-Fi conectado!"
- Deve começar a enviar dados a cada 5 segundos

### **3. Verificar Aplicação Web:**
- Acesse: `http://10.10.14.223:3000`
- Faça login
- Vá para o Dashboard
- Deve mostrar dados de SpO2 em tempo real

## 🚨 **Solução de Problemas**

### **Wi-Fi não conecta:**
- Verifique SSID e senha
- Verifique sinal da rede
- Reinicie o ESP32

### **Dados não aparecem na web:**
- Verifique se o backend está rodando
- Verifique o IP do servidor no código
- Verifique logs do Serial Monitor

### **ESP-NOW não funciona:**
- Verifique se ambos ESP32 estão na mesma rede
- Verifique se o MAC está correto
- Reinicie ambos os dispositivos

## 📱 **Monitoramento**

### **Logs do Receptor:**
```
📊 Dados recebidos via ESP-NOW: SpO2: 98% | Timestamp: 2025-09-27 12:00:00
✅ Dados enviados para web: {"success":true,"message":"Dados recebidos com sucesso"}
```

### **Logs do Transmissor:**
```
Wi-Fi conectado!
IP: 10.10.14.224
SpO2: 98% | Timestamp: 2025-09-27 12:00:00
✅ Dados enviados: {"success":true,"message":"Dados recebidos com sucesso"}
```

## 🔧 **Configurações Avançadas**

### **Intervalo de Envio:**
```cpp
delay(5000); // Altere para o intervalo desejado (em ms)
```

### **Faixa de Valores SpO2:**
```cpp
spo2 = map(spo2, 0, 4095, 90, 100); // Ajuste conforme seu sensor
```

### **Pino do Sensor:**
```cpp
#define SPO2_PIN A0 // Altere conforme sua conexão
```

---

**Desenvolvido para o TCC em Automação Industrial - ETEC Jorge Street**
