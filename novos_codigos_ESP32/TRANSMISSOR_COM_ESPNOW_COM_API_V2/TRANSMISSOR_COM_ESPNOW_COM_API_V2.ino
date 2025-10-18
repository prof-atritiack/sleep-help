#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <esp_now.h>
#include <esp_wifi.h>
// 💡 NOVO: Biblioteca do Sensor MAX3010X
#include "MAX30105.h" 
#include "heartRate.h" 

// --- CONFIGURAÇÕES DE HARDWARE E PINOS ---
#define BUZZER_PIN 4
#define LED_PIN 2

// --- CONFIGURAÇÕES DE COMUNICAÇÃO (TIMERS OTIMIZADOS) ---
#define INTERVALO_NOW 5000         // Leitura e envio ESP-NOW (5 segundos)
#define DELAY_API_APOS_NOW 3000    // Envio da API ocorre 3 segundos após o NOW
#define SENSOR_READ_DELAY 100      // Delay entre leituras do sensor

// Configurações de rede
const char* ssid = "aptodaIsabella";    
const char* password = "bellabella1302";       
const char* serverIP = "192.168.1.6"; // <--- VERIFIQUE O IP DO SEU COMPUTADOR AQUI
const char* apiPort = "3001";
const String serverURL = "http://" + String(serverIP) + ":" + String(apiPort) + "/api/sensor-data";

// MAC do receptor ESP32
uint8_t broadcastAddress[] = {0xD8, 0xBC, 0x38, 0xF9, 0xD0, 0x54};

// Struct para ESP-NOW
typedef struct {
  int id;
  bool ledOn; // True se houver alerta (SpO2 <= 95)
} mensagem;
mensagem dados;

// Variáveis de estado
uint8_t wifiChannel = 0; 
unsigned long ultimoEnvioNOW = 0;
unsigned long agendarEnvioAPI = 0; 
int spo2Atual = 0; // Usado para o valor real do sensor
int heartRateAtual = 0; // Novo valor para a taxa de batimentos
int contadorMedicoes = 0;
volatile bool now_ack_received = false; 

// 💡 NOVO: Objeto do Sensor
MAX30105 particleSensor;

TFT_eSPI tft = TFT_eSPI();

// --- FUNÇÕES DE CALLBACKS E UTILIADE ---

void onSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  now_ack_received = (status == ESP_NOW_SEND_SUCCESS);
  if (now_ack_received) {
    Serial.println("[NOW] ✅ Pacote enviado com ACK (Sucesso Confirmado).");
  } else {
    Serial.println("[NOW] ❌ Falha de ACK (Dados Perdidos).");
  }
}

void conectarWiFi() {
  WiFi.mode(WIFI_STA);
  
  Serial.println("[WIFI] Limpando credenciais antigas da Flash...");
  WiFi.disconnect(true, true);
  delay(100);
  
  WiFi.begin(ssid, password);
  
  int tentativas = 0;
  Serial.print("[WIFI] Conectando");
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("[WIFI] ✅ Wi-Fi conectado!");
    
    wifiChannel = WiFi.channel();
    
    if (esp_wifi_set_ps(WIFI_PS_NONE) == ESP_OK) {
        Serial.println("[WIFI] ⭐ Power Save desativado para estabilidade do NOW.");
    }
    
    if (esp_wifi_set_channel(wifiChannel, WIFI_SECOND_CHAN_NONE) == ESP_OK) {
        Serial.printf("[WIFI/NOW] Canal sincronizado para %d\n", wifiChannel);
    } else {
        Serial.println("[WIFI/NOW] ❌ Falha ao sincronizar o canal.");
    }
    
  } else {
    Serial.println();
    Serial.println("[WIFI] ❌ Falha ao conectar Wi-Fi! Verifique as credenciais.");
    wifiChannel = 0; 
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

// 💡 NOVO: Função de Leitura do Sensor
void lerSensor() {
  // Configuração para leituras médias.
  // IMPORTANTE: Mantenha o dedo firme no sensor durante este período.
  
  long irValue = particleSensor.get  IR();
  
  // Limpa valores anteriores
  spo2Atual = 0; 
  heartRateAtual = 0;

  if (checkFor  Beat(irValue) == true) {
    // Calcula HR (apenas para fins de log e display, o SPO2 será o principal)
    long beatAvg = get_average_rate();
    if (beatAvg > 0) {
      heartRateAtual = (int)beatAvg;
    }
  }

  if (irValue < 50000) { // Limite baixo para indicar que o dedo não está lá
    Serial.println("[SENSOR] ⚠️ Nenhum dedo detectado ou sinal fraco.");
    spo2Atual = 0;
    heartRateAtual = 0;
    
  } else {
    // Implementação simplificada de leitura de SpO2 da biblioteca
    float spo2 = 0;
    float hr = 0;
    
    // Calcula SPO2
    if (particleSensor.calcSPO2(particleSensor.get  Red(), irValue, &spo2, &hr)) {
        spo2Atual = (int)spo2;
        // O valor de HR da calcSPO2 é mais preciso para o momento
        if (hr > 0) heartRateAtual = (int)hr;
        Serial.printf("[SENSOR] Leitura: SpO2=%d%%, HR=%d bpm\n", spo2Atual, heartRateAtual);
    } else {
        Serial.println("[SENSOR] Falha na leitura do SpO2.");
    }
  }

  particleSensor.nextSample(); // Prepara o sensor para a próxima leitura
}

void executarEnvioAPI(int spo2, int heartRate) {
  if (WiFi.status() != WL_CONNECTED) {
    tft.fillRect(0, 150, tft.width(), 30, TFT_BLACK);
    tft.setCursor(10, 155);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("API FALHOU: SEM WIFI");
    Serial.println("[API] ❌ Wi-Fi desconectado! Pulando POST.");
    agendarEnvioAPI = 0; 
    return;
  }
  
  Serial.println("\n[API] 📊 Enviando dados (Sequencial)...");
  Serial.println("      SpO2: " + String(spo2) + "% | HR: " + String(heartRate) + " bpm");
  
  HTTPClient http;
  http.begin(serverURL.c_str()); 
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(10000); 
  
  DynamicJsonDocument doc(1024);
  doc["spo2"] = spo2;
  doc["heartRate"] = heartRate; // Novo campo
  doc["timestamp"] = getCurrentTime();
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  int httpResponseCode = http.POST(jsonString);
  
  tft.fillRect(0, 150, tft.width(), 30, TFT_BLACK);
  tft.setCursor(10, 155);

  if (httpResponseCode > 0) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.print("API: SUCESSO! C:");
    tft.print(httpResponseCode);
    Serial.println("[API] ✅ Envio Sucesso. Código: " + String(httpResponseCode));
    piscarLED(1, 100);
  } else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("API: FALHOU! C:");
    tft.print(httpResponseCode);
    Serial.println("[API] ❌ Falha no envio. Código: " + String(httpResponseCode) + " (" + http.errorToString(httpResponseCode) + ")");
    piscarLED(5, 100);
  }
  
  http.end();
  agendarEnvioAPI = 0;
}

// --- SETUP ---
void setup() {
  Serial.begin(115200);
  Wire.begin(); // Inicializa I2C
  
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
  tft.setCursor(10, 35);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.print("--- SENSOR INICIALIZANDO ---"); 
  
  // 💡 NOVO: Inicialização do Sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("========================================");
    Serial.println("  ❌ ERRO CRÍTICO: MAX3010x não encontrado!");
    Serial.println("  Verifique as conexões I2C (SDA=21, SCL=22).");
    Serial.println("========================================");
    tft.fillRect(0, 70, tft.width(), 100, TFT_BLACK);
    tft.setCursor(10, 75);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("ERRO: SENSOR OFFLINE!");
    while (true);
  }
  particleSensor.setup(); // Configuração padrão para batimentos e oximetria
  particleSensor.setPulseAmplitudeRed(0x0A); // Ajusta a intensidade do LED Vermelho
  Serial.println("[SENSOR] Sensor MAX3010x inicializado com sucesso.");

  // Conexão inicial e setup de canal/power save
  conectarWiFi(); 

  if (wifiChannel == 0) {
     Serial.println("========================================");
     Serial.println("  ❌ ERRO CRÍTICO: Wi-Fi inicial falhou. PARADA FORÇADA.");
     Serial.println("========================================");
     tft.fillRect(0, 70, tft.width(), 100, TFT_BLACK);
     tft.setCursor(10, 75);
     tft.setTextColor(TFT_RED, TFT_BLACK);
     tft.print("ERRO: WIFI OFFLINE!");
     while (true); 
  }

  // Inicialização do ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("[NOW] ❌ Erro ao iniciar ESP-NOW");
    while (true);
  }
  esp_now_register_send_cb(onSent);

  // Adicionar Peer
  esp_now_peer_info_t peerInfo = {};
  peerInfo.channel = wifiChannel; 
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[NOW] ❌ Erro ao adicionar peer");
    while (true);
  }
  
  dados.id = 1;
  dados.ledOn = false; 
  
  Serial.println("========================================");
  Serial.println("  SISTEMA PRONTO: Leitura -> NOW -> API");
  Serial.println("========================================");
}

// --- LOOP ---
void loop() {
  
  // 💡 NOVO: Continua lendo o sensor em alta frequência para coletar dados
  lerSensor(); 
  delay(SENSOR_READ_DELAY); // Espera 100ms antes da próxima leitura do sensor

  // 1. CHECA E EXECUTA O CICLO DE ENVIO (baseado no tempo INTERVALO_NOW)
  if (millis() - ultimoEnvioNOW >= INTERVALO_NOW) {
    
    // LÓGICA DE ALERTA: SpO2 abaixo de 95% ou sensor não detectando (SpO2=0)
    bool alerta = (spo2Atual <= 95 && spo2Atual > 0) || (spo2Atual == 0 && heartRateAtual == 0);
    
    // Prepara o pacote NOW
    dados.ledOn = alerta; 
    now_ack_received = false; 
    
    // ENVIO ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *)&dados, sizeof(dados));

    // Pequeno delay para que o callback seja disparado
    delay(50); 
    
    // Log e TFT do resultado NOW
    if (now_ack_received) {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.setCursor(10, 190);
        tft.print("NOW: DADOS ENVIADOS (ACK!)");
    } else {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.setCursor(10, 190);
        tft.print("NOW: FALHA NO ENVIO (SEM ACK!)");
    }

    // AGENDA O ENVIO DA API
    // Se o sensor não estiver detectando (Spo2=0, HR=0), não há dados válidos para a API
    if (spo2Atual > 0 || heartRateAtual > 0) {
        agendarEnvioAPI = millis() + DELAY_API_APOS_NOW;
    } else {
        agendarEnvioAPI = 0; // Não envia dados inválidos
    }


    // --- ATUALIZAÇÃO DO DISPLAY E ESTADO LOCAL ---
    digitalWrite(BUZZER_PIN, alerta); 
    digitalWrite(LED_PIN, alerta);   

    tft.fillRect(0, 70, tft.width(), 30, TFT_BLACK);
    tft.setCursor(10, 75);
    tft.setTextColor(TFT_PINK, TFT_BLACK);
    tft.print("SpO2: ");
    tft.print(spo2Atual);
    tft.print(" %");
    
    tft.fillRect(0, 105, tft.width(), 30, TFT_BLACK);
    tft.setCursor(10, 115);
    tft.setTextColor(alerta ? TFT_RED : TFT_GREEN, TFT_BLACK);
    tft.print("HR: ");
    tft.print(heartRateAtual);
    tft.print(" bpm");
    
    tft.fillRect(0, 130, tft.width(), 20, TFT_BLACK);
    tft.setCursor(10, 135);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print("Status Geral: " + String(alerta ? "ALERTA" : "NORMAL"));
    
    ultimoEnvioNOW = millis();
  }

  // 2. CHECA E EXECUTA O ENVIO DA API (Sequencial)
  if (agendarEnvioAPI != 0 && millis() >= agendarEnvioAPI) {
    executarEnvioAPI(spo2Atual, heartRateAtual);
  }
}