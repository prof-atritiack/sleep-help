#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <esp_now.h>
#include <esp_wifi.h>

// --- CONFIGURAÇÕES DE HARDWARE E PINOS ---
#define BUZZER_PIN 4
#define LED_PIN 2

// --- CONFIGURAÇÕES DE COMUNICAÇÃO (TIMERS OTIMIZADOS) ---
#define INTERVALO_NOW 5000         // Leitura e envio ESP-NOW (5 segundos)
#define DELAY_API_APOS_NOW 3000    // Envio da API ocorre 3 segundos após o NOW

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
int spo2Atual = 0;
int contadorMedicoes = 0;
volatile bool now_ack_received = false; 

TFT_eSPI tft = TFT_eSPI();

// --- FUNÇÕES DE CALLBACKS E UTILIADE ---

void onSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  now_ack_received = (status == ESP_NOW_SEND_SUCCESS);
  if (now_ack_received) {
    Serial.println("[NOW] ✅ Pacote enviado com ACK (Sucesso Confirmado).");
  } else {
    // Mantendo este log agora, pois o Power Save Desativado deve evitar falhas
    // Se falhar, é uma falha REAL de comunicação.
    Serial.println("[NOW] ❌ Falha de ACK (Dados Perdidos).");
  }
}

void conectarWiFi() {
  WiFi.mode(WIFI_STA);
  
  // 💡 FIX DE CREDENCIAIS: Limpa a memória flash antes de tentar conectar.
  Serial.println("[WIFI] Limpando credenciais antigas da Flash...");
  WiFi.disconnect(true, true); // (true: desliga, true: limpa credenciais salvas)
  delay(100);
  
  WiFi.begin(ssid, password);
  
  int tentativas = 0;
  Serial.print("[WIFI] Conectando");
  // Reduzindo o número de tentativas para falhar mais rápido se as credenciais estiverem erradas
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500); // Meio segundo de delay
    Serial.print(".");
    tentativas++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("[WIFI] ✅ Wi-Fi conectado!");
    
    // Tenta descobrir e aplicar o canal (Coexistência FIX)
    wifiChannel = WiFi.channel();
    
    // SOLUÇÃO ESTÁVEL: DESATIVA POWER SAVE PARA GARANTIR ESTABILIDADE DO RÁDIO
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

void executarEnvioAPI(int spo2) {
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
  Serial.println("      SpO2: " + String(spo2) + "%");
  
  HTTPClient http;
  http.begin(serverURL.c_str()); 
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(10000); 
  
  DynamicJsonDocument doc(1024);
  doc["spo2"] = spo2;
  doc["timestamp"] = getCurrentTime();
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  int httpResponseCode = http.POST(jsonString);
  
  // Atualização do TFT para refletir o status da API
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
  randomSeed(analogRead(0));
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
  tft.print("--- FIX: LIMPEZA DE FLASH ---"); 
  
  // Conexão inicial e setup de canal/power save
  conectarWiFi(); 

  if (wifiChannel == 0) {
     Serial.println("========================================");
     Serial.println("  ❌ ERRO CRÍTICO: Wi-Fi inicial falhou.");
     Serial.println("  VERIFIQUE SSID E SENHA! A placa está parada.");
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
  peerInfo.channel = wifiChannel; // Usa o canal descoberto
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[NOW] ❌ Erro ao adicionar peer");
    while (true);
  }
  
  dados.id = 1;
  dados.ledOn = false; 
  
  Serial.println("========================================");
  Serial.println("  MODO ESTÁVEL: Power Save Wi-Fi Desativado.");
  Serial.println("========================================");
}

// --- LOOP ---
void loop() {
  
  // 1. CHECA E EXECUTA O CICLO DE LEITURA E ENVIO NOW
  if (millis() - ultimoEnvioNOW >= INTERVALO_NOW) {
    
    // SIMULAÇÃO DE LEITURA
    spo2Atual = random(88, 101);
    bool alerta = (spo2Atual <= 95);
    
    // Prepara o pacote NOW
    dados.ledOn = alerta; 
    now_ack_received = false; 
    
    // ENVIO ESP-NOW: O Power Save desativado deve garantir o ACK.
    esp_now_send(broadcastAddress, (uint8_t *)&dados, sizeof(dados));

    // Damos um pequeno tempo para que o callback seja disparado
    delay(50); 
    
    // Log do resultado NOW no Serial
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
    agendarEnvioAPI = millis() + DELAY_API_APOS_NOW;

    // --- ATUALIZAÇÃO DO DISPLAY E ESTADO LOCAL ---
    digitalWrite(BUZZER_PIN, alerta); 
    digitalWrite(LED_PIN, alerta);   

    tft.fillRect(0, 70, tft.width(), 30, TFT_BLACK);
    tft.setCursor(10, 75);
    tft.setTextColor(TFT_PINK, TFT_BLACK);
    tft.print("SpO2 SIM: ");
    tft.print(spo2Atual);
    tft.print(" %");
    
    tft.fillRect(0, 105, tft.width(), 30, TFT_BLACK);
    tft.setCursor(10, 115);
    tft.setTextColor(alerta ? TFT_RED : TFT_GREEN, TFT_BLACK);
    tft.print("STATUS: " + String(alerta ? "ALERTA" : "NORMAL"));
    
    ultimoEnvioNOW = millis();
  }

  // 2. CHECA E EXECUTA O ENVIO DA API (Sequencial)
  if (agendarEnvioAPI != 0 && millis() >= agendarEnvioAPI) {
    executarEnvioAPI(spo2Atual);
  }
}