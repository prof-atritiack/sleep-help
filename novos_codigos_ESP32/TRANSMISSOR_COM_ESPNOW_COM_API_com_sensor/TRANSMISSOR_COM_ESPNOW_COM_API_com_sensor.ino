#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <esp_now.h>
#include <esp_wifi.h>
// DEPENDÊNCIAS DO SENSOR
#include <MAX30105.h>
#include <spo2_algorithm.h> 

// --- CONFIGURAÇÕES DE HARDWARE E PINOS ---
#define BUZZER_PIN 4
#define LED_PIN 2

// --- CONFIGURAÇÕES DO SENSOR ---
#define MAX_SPO2_BUFFER 100        // Buffer para o cálculo do SpO2
#define MIN_IR_DETECT 30000        // Limiar para considerar o dedo presente

// --- CONFIGURAÇÕES DE COMUNICAÇÃO (TIMERS OTIMIZADOS) ---
#define INTERVALO_NOW 5000         // Envio ESP-NOW (5 segundos)
#define DELAY_API_APOS_NOW 3000    // Envio da API ocorre 3 segundos após o NOW
#define INTERVALO_LEITURA 10       // Pequeno delay no loop para evitar loop infinito na leitura

// Configurações de rede
const char* ssid = "STREET_WIFI_SM"; 
const char* password = "StreetFI";       
const char* serverIP = "10.10.14.223"; // <--- VERIFIQUE O IP DO SEU COMPUTADOR AQUI
const char* apiPort = "3001";
const String serverURL = "http://" + String(serverIP) + ":" + String(apiPort) + "/api/sensor-data";

// MAC do receptor ESP32
uint8_t broadcastAddress[] = {0xF4, 0x65, 0x0B, 0x57, 0xBF, 0xF8};

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
volatile bool now_ack_received = false; 

// Variáveis do Sensor
MAX30105 particleSensor;
TFT_eSPI tft = TFT_eSPI();

// Variáveis para o algoritmo
uint32_t irBuffer[MAX_SPO2_BUFFER];
uint32_t redBuffer[MAX_SPO2_BUFFER];
int contadorMedicoes = 0; 

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
    Serial.println("[WIFI] IP Local: " + WiFi.localIP().toString());
    
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

void lerSensorECalcularSpO2() {
  
  // 1. CHECA SE O DEDO ESTÁ PRESENTE
  long irAvg = 0;
  int nTest = 5;
  for (int i = 0; i < nTest; i++) {
    while (!particleSensor.available()) particleSensor.check();
    irAvg += particleSensor.getIR();
    particleSensor.nextSample(); 
    delay(5);
  }
  irAvg /= nTest;

  // Limpa a área de status anterior
  tft.fillRect(0, 40, tft.width(), 40, TFT_BLACK);

  if (irAvg > MIN_IR_DETECT) {
    // 2. DEDO PRESENTE: PREENCHE OS BUFFERS PARA CÁLCULO
    Serial.println("[SENSOR] Dedo detectado. Coletando amostras...");
    
    // Coleta MAX_SPO2_BUFFER amostras (esta é a parte mais lenta)
    for (int i = 0; i < MAX_SPO2_BUFFER; i++) {
      while (!particleSensor.available()) particleSensor.check();
      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample();
    }
    
    // 3. CALCULA O SPO2
    int32_t calculatedSpO2;
    int8_t valid_spo2;
    int32_t heartRateDummy; 
    int8_t valid_hrDummy; 

    // Chama o algoritmo
    maxim_heart_rate_and_oxygen_saturation(irBuffer, MAX_SPO2_BUFFER, redBuffer,
                                          &calculatedSpO2, &valid_spo2, &heartRateDummy, &valid_hrDummy);

    // 4. ATUALIZA O ESTADO
    if (valid_spo2) {
      spo2Atual = (int)calculatedSpO2;
      contadorMedicoes++;
      
      // Log no Serial
      Serial.printf("[SENSOR] ✅ Medição #%d - SpO2: %d%%\n", contadorMedicoes, spo2Atual);

      // Atualizar display
      tft.setCursor(10, 45);
      tft.setTextColor(TFT_PINK, TFT_BLACK);
      tft.print("SpO2: ");
      tft.print(spo2Atual);
      tft.print(" %");
      tft.fillRect(0, 80, tft.width(), 30, TFT_BLACK); // Limpa área extra
      
    } else {
      spo2Atual = 0; // Medição inválida, mas dedo presente (movimento, etc.)
      Serial.println("[SENSOR] ⚠️ Medição inválida (dedo em movimento?)");

      // Atualizar display
      tft.setCursor(10, 45);
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.print("Medicao Invalida");
      tft.fillRect(0, 80, tft.width(), 30, TFT_BLACK);
    }
    
  } else {
    // 5. DEDO AUSENTE
    spo2Atual = 0;
    Serial.println("[SENSOR] Coloque o dedo no sensor");
    
    // Atualizar display
    tft.setCursor(10, 45);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("Coloque o dedo");
    tft.fillRect(0, 80, tft.width(), 30, TFT_BLACK);
  }
}

// --- SETUP ---
void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  Wire.begin(); 
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

  // Inicializar sensor MAX30105 
  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("❌ ERRO CRÍTICO: Sensor MAX30102 nao encontrado!");
    tft.fillRect(0, 70, tft.width(), 100, TFT_BLACK);
    tft.setCursor(10, 75);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("ERRO: Sensor OFFLINE!");
    while (true);
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeIR(0x1F);
  
  Serial.println("[SENSOR] Sensor MAX30105 inicializado com sucesso.");
  
  // Conexão inicial e setup de canal/power save
  conectarWiFi(); 

  if (wifiChannel == 0) {
      Serial.println("❌ ERRO CRÍTICO: Wi-Fi inicial falhou. Parada Forçada.");
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
  
  // 1. CHECA E EXECUTA O CICLO DE LEITURA E ENVIO NOW
  if (millis() - ultimoEnvioNOW >= INTERVALO_NOW) {
    
    // EXECUTA A LEITURA E CÁLCULO REAL DO SPO2
    lerSensorECalcularSpO2();

    // Lógica de Alerta: SpO2 <= 95 E a medição foi válida (SpO2 > 0)
    bool alerta = (spo2Atual <= 95 && spo2Atual > 0);
    
    // Prepara o pacote NOW
    dados.ledOn = alerta; 
    now_ack_received = false; 
    
    // ENVIO ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *)&dados, sizeof(dados));

    delay(50); 
    
    // Log do resultado NOW no TFT
    if (now_ack_received) {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.setCursor(10, 190);
        tft.print("NOW: DADOS ENVIADOS (ACK!)");
    } else {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.setCursor(10, 190);
        tft.print("NOW: FALHA NO ENVIO (SEM ACK!)");
    }

    // Só agenda a API se houver um valor válido
    if (spo2Atual > 0) {
      agendarEnvioAPI = millis() + DELAY_API_APOS_NOW;
    } else {
      agendarEnvioAPI = 0; // Não envia dados inválidos
    }

    // --- ATUALIZAÇÃO DO DISPLAY E ESTADO LOCAL ---
    // Uso de ternário para evitar warnings
    digitalWrite(BUZZER_PIN, (alerta ? HIGH : LOW)); 
    digitalWrite(LED_PIN, (alerta ? HIGH : LOW));    
    
    // Atualiza status geral no TFT
    tft.fillRect(0, 110, tft.width(), 30, TFT_BLACK);
    tft.setCursor(10, 115);
    tft.setTextColor(alerta ? TFT_RED : TFT_GREEN, TFT_BLACK);
    tft.print("STATUS: " + String(alerta ? "ALERTA" : "NORMAL"));
    
    ultimoEnvioNOW = millis();
  }

  // 2. CHECA E EXECUTA o ENVIO DA API (Sequencial)
  if (agendarEnvioAPI != 0 && millis() >= agendarEnvioAPI) {
    executarEnvioAPI(spo2Atual);
  }
  
  // Pequeno delay para evitar loop infinito na leitura, mantendo responsividade
  delay(INTERVALO_LEITURA); 
}