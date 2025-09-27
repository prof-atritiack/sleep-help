# 📡 ESP32 Oximetro + API - Código Mesclado

## 🎯 Descrição

Este código combina o sensoriamento real do MAX30105 com o envio de dados para a API da aplicação Sleep Help. É a versão completa que integra hardware real com a interface web.

## 🚀 Funcionalidades

### Sensoriamento:
- ✅ **Sensor MAX30105** para medições reais de SpO2 e BPM
- ✅ **Validação de medições** com algoritmos de oxímetro
- ✅ **Detecção de dedo** no sensor
- ✅ **Cálculo automático** de saturação e frequência cardíaca

### Comunicação:
- ✅ **ESP-NOW** para comunicação com receptor
- ✅ **Wi-Fi HTTP** para envio à API web
- ✅ **Reconexão automática** de Wi-Fi
- ✅ **Envio em intervalos** configuráveis

### Interface:
- ✅ **Display TFT** com dados em tempo real
- ✅ **LED indicador** de status
- ✅ **Buzzer** para alertas
- ✅ **Serial Monitor** com logs detalhados

## 📁 Arquivos

- `TRANSMISSOR_ESP32_V3_API.ino` - Código principal mesclado
- `CONFIGURACAO_API.txt` - Instruções de configuração
- `README_API.md` - Este arquivo

## 🔧 Configuração Rápida

1. **Conecte o sensor MAX30105**:
   - VCC → 3.3V
   - GND → GND
   - SDA → GPIO 21
   - SCL → GPIO 22

2. **Configure as credenciais Wi-Fi** no código

3. **Verifique o IP do servidor** (172.20.10.7:3001)

4. **Faça upload** para o ESP32

5. **Abra o Serial Monitor** (115200 baud)

6. **Coloque o dedo no sensor** e veja os dados!

## 📊 Exemplo de Saída

```
========================================
    SLEEP HELP - ESP32 Oximetro
    Sensor MAX30105 + API + ESP-NOW
========================================
Enviando dados para: http://172.20.10.7:3001/api/sensor-data
========================================
✅ Wi-Fi conectado!
IP: 172.20.10.8
RSSI: -45 dBm
Valor medio IR: 45000
Medicao #1 - SpO2: 98%  HR: 72 bpm
📊 Enviando dados para API...
   SpO2: 98%
   Timestamp: 2025-09-27 12:30:45
✅ Dados enviados para API: {"success":true}
✅ Saturação normal
----------------------------------------
```

## 🎛️ Controles

- **Display TFT**: Mostra SpO2, BPM e status da API
- **LED interno**: Indica status das operações
- **Buzzer**: Alerta para baixa saturação
- **Serial Monitor**: Logs detalhados de todas as operações

## 🔗 Integração Completa

Este código se integra com:
- **Backend Sleep Help** (Node.js) via HTTP
- **Frontend Sleep Help** (React) via WebSocket
- **Receptor ESP32** via ESP-NOW
- **Sistema de alertas** automático

## 🚨 Alertas Automáticos

- **Baixa saturação** (< 95%): LED pisca 3x + buzzer
- **Medição inválida**: LED pisca 5x
- **Erro de conexão**: Reconexão automática
- **Dedo não detectado**: Instruções no display

## 📱 Monitoramento Web

1. Acesse: `http://172.20.10.7:3000`
2. Faça login
3. Vá para o Dashboard
4. Veja os dados reais do sensor chegando!

## 🛠️ Personalização

### Intervalo de envio:
```cpp
#define INTERVALO_ENVIO 5000  // 5 segundos
```

### Sensibilidade do sensor:
```cpp
#define DEDO_THRESHOLD 30000  // Ajuste conforme necessário
```

### Pinos do hardware:
```cpp
#define BUZZER_PIN 4
#define LED_PIN 2
```

## 🔧 Bibliotecas Necessárias

- MAX30105 (sensor de oxímetro)
- TFT_eSPI (display)
- WiFi (conexão Wi-Fi)
- HTTPClient (envio HTTP)
- ArduinoJson (formatação JSON)

---

**Desenvolvido para o TCC em Automação Industrial - ETEC Jorge Street**
