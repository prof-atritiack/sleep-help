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
#define VIBRA_PIN 4 // Pino do Vibrador
#define LED_PIN 2 // Pino do LED

// --- CONFIGURAÇÕES DO SENSOR ---
#define MAX_SPO2_BUFFER 100        // Buffer para o cálculo do SpO2
#define MIN_IR_DETECT 30000        // Limiar para considerar o dedo presente
#define MODO_TESTE_SENSOR true     // true = simula valores quando sensor offline, false = usa sensor real

// --- CONFIGURAÇÕES DE COMUNICAÇÃO (SEQUENCIAL) ---
#define DELAY_APOS_DESCONECTAR_WIFI 2000  // Delay de 2 segundos após desconectar WiFi
#define DELAY_APOS_ESPNOW 2000            // Delay de 2 segundos após enviar ESP-NOW

// Configurações de rede
// const char* ssid = "STREET_WIFI_SM"; 
// const char* password = "StreetFI";   
const char* ssid = "aptodaIsabella"; 
const char* password = "bellabella1302";     
const char* serverIP = "192.168.1.6"; // <--- VERIFIQUE O IP DO SEU COMPUTADOR AQUI
const char* apiPort = "3001";
const String serverURL = "http://" + String(serverIP) + ":" + String(apiPort) + "/api/sensor-data";

// MAC do receptor ESP32
// uint8_t broadcastAddress[] = {0xF4, 0x65, 0x0B, 0x57, 0xBF, 0xF8};
// uint8_t broadcastAddress[] = {0xD8, 0xBC, 0x38, 0xF9, 0xD0, 0x54};
uint8_t broadcastAddress[] = {0xA0, 0xDD, 0x6C, 0x86, 0x14, 0x34};

// Struct para ESP-NOW
typedef struct {
  int id;
  bool ledOn; // True se houver alerta (SpO2 <= 95)
} mensagem;
mensagem dados;

// Variáveis de estado
uint8_t wifiChannel = 0; 
int spo2Atual = 0;
bool sensorDisponivel = false; // Flag para indicar se o sensor está disponível
int contadorTeste = 0; // Contador para alternar valores no modo teste
bool alertaAtivo = false; // Flag para controlar LED piscante
bool alertaAnterior = false; // Flag para detectar mudança de estado
bool ledPiscando = false; // Flag para indicar que LED está piscando
int contadorPiscadas = 0; // Contador de piscadas
unsigned long ultimoPiscarLED = 0; // Controle de timing do LED piscante 

// Variáveis do Sensor
MAX30105 particleSensor;
TFT_eSPI tft = TFT_eSPI();

// Variáveis para o algoritmo
uint32_t irBuffer[MAX_SPO2_BUFFER];
uint32_t redBuffer[MAX_SPO2_BUFFER];
int contadorMedicoes = 0; 

// --- FUNÇÕES DE CALLBACKS E UTILIADE ---

void conectarWiFi() {
  WiFi.mode(WIFI_STA);
  Serial.println("[WIFI] Conectando...");
  // Não apaga credenciais para não interferir com ESP-NOW
  WiFi.disconnect(false); 
  delay(100);
  yield(); // Alimenta watchdog
  
  WiFi.begin(ssid, password);
  
  int tentativas = 0;
  Serial.print("[WIFI] Conectando");
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    yield(); // Alimenta o watchdog
    Serial.print(".");
    tentativas++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("[WIFI] ✅ Wi-Fi conectado!");
    Serial.println("[WIFI] IP Local: " + WiFi.localIP().toString());
    
    wifiChannel = WiFi.channel();
    
    if (esp_wifi_set_ps(WIFI_PS_NONE) == ESP_OK) {
        Serial.println("[WIFI] ⚡ Power Save desativado");
    }
    
    if (esp_wifi_set_channel(wifiChannel, WIFI_SECOND_CHAN_NONE) == ESP_OK) {
        Serial.printf("[WIFI] ✅ Canal sincronizado: %d\n", wifiChannel);
    } else {
        Serial.println("[WIFI] ❌ Falha ao sincronizar canal");
    }
    
  } else {
    Serial.println();
    Serial.println("[WIFI] ❌ Falha ao conectar - verifique credenciais");
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

bool executarEnvioAPI(int spo2) {
  if (WiFi.status() != WL_CONNECTED) {
    tft.fillRect(0, 150, tft.width(), 30, TFT_BLACK);
    tft.setCursor(10, 155);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("API FALHOU: SEM WIFI");
    Serial.println("[API] ❌ WiFi desconectado - pulando envio");
    return false;
  }
  
  Serial.println("[API] 📡 Enviando dados para servidor...");
  Serial.printf("[API]    SpO2: %d%%\n", spo2);
  Serial.printf("[API]    Timestamp: %s\n", getCurrentTime().c_str());
  
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
    Serial.printf("[API] ✅ Envio bem-sucedido - HTTP %d\n", httpResponseCode);
    piscarLED(1, 100);
    http.end();
    return true;
  } else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("API: FALHOU! C:");
    tft.print(httpResponseCode);
    Serial.printf("[API] ❌ Falha no envio - HTTP %d\n", httpResponseCode);
    Serial.printf("[API]    Erro: %s\n", http.errorToString(httpResponseCode).c_str());
    piscarLED(5, 100);
    http.end();
    return false;
  }
}

void enviarESPNow(int spo2) {
  // Prepara e envia ESP-NOW
  dados.ledOn = (spo2 <= 95 && spo2 > 0);
  dados.id = contadorMedicoes;
  
  Serial.println("[NOW] 📡 Enviando dados via ESP-NOW...");
  Serial.printf("[NOW]    ID: %d\n", dados.id);
  Serial.printf("[NOW]    SpO2: %d%%\n", spo2);
  Serial.printf("[NOW]    Alerta: %s\n", dados.ledOn ? "🚨 ATIVO" : "✅ NORMAL");
  
  esp_now_send(broadcastAddress, (uint8_t *)&dados, sizeof(dados));
  
  Serial.println("[NOW] ✅ Dados enviados com sucesso");
  
  // Atualiza TFT
  tft.fillRect(0, 190, tft.width(), 20, TFT_BLACK);
  tft.setCursor(10, 190);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print("NOW: ENVIADO");
}

void lerSensorECalcularSpO2() {
  
  // Se o sensor não está disponível, usa modo de teste se habilitado
  if (!sensorDisponivel) {
    if (MODO_TESTE_SENSOR) {
      // Modo de teste: simula valores alternados para testar transmissão
      contadorTeste++;
      
      // Alterna entre valores normais (98%) e de alerta (92%) a cada ciclo
      if (contadorTeste % 2 == 0) {
        spo2Atual = 98; // Valor normal
      } else {
        spo2Atual = 92; // Valor de alerta (para testar o alarme)
      }
      
      contadorMedicoes++;
      
      // Atualiza display
      tft.fillRect(0, 40, tft.width(), 40, TFT_BLACK);
      tft.setCursor(10, 45);
      tft.setTextColor(TFT_CYAN, TFT_BLACK);
      tft.print("MODO TESTE - SpO2: ");
      tft.print(spo2Atual);
      tft.print("%");
      
      Serial.printf("[SENSOR] 📊 Medição simulada #%d - SpO2: %d%%\n", contadorMedicoes, spo2Atual);
      return;
    } else {
      // Sem sensor e sem modo teste: apenas retorna
      spo2Atual = 0;
      tft.fillRect(0, 40, tft.width(), 40, TFT_BLACK);
      tft.setCursor(10, 45);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.print("Sensor offline");
      return;
    }
  }
  
  // 1. CHECA SE O DEDO ESTÁ PRESENTE
  particleSensor.check(); // Atualiza estado do sensor
  
  long irAvg = 0;
  int nTest = 5;
  int amostrasValidas = 0;
  
  // Coleta amostras com pequeno delay para dar tempo ao sensor
  for (int i = 0; i < nTest; i++) {
    particleSensor.check(); // Atualiza estado antes de verificar
    if (particleSensor.available()) {
      irAvg += particleSensor.getIR();
      particleSensor.nextSample();
      amostrasValidas++;
    }
    delay(10); // Pequeno delay para dar tempo ao sensor
  }
  
  // Se não coletou amostras suficientes, assume sensor offline
  if (amostrasValidas == 0) {
    spo2Atual = 0;
    Serial.println("[SENSOR] ⚠️ Sensor offline - sem amostras válidas");
    tft.fillRect(0, 40, tft.width(), 40, TFT_BLACK);
    tft.setCursor(10, 45);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("Sensor offline");
    return;
  }
  
  irAvg /= amostrasValidas;

  // Limpa a área de status anterior
  tft.fillRect(0, 40, tft.width(), 40, TFT_BLACK);

  if (irAvg > MIN_IR_DETECT) {
    // 2. DEDO PRESENTE: PREENCHE OS BUFFERS PARA CÁLCULO
    Serial.println("[SENSOR] 👆 Dedo detectado - coletando amostras...");
    
    // Coleta MAX_SPO2_BUFFER amostras (com delay para dar tempo ao sensor)
    int amostrasColetadas = 0;
    for (int i = 0; i < MAX_SPO2_BUFFER; i++) {
      particleSensor.check(); // Atualiza estado antes de verificar
      if (particleSensor.available()) {
        redBuffer[i] = particleSensor.getRed();
        irBuffer[i] = particleSensor.getIR();
        particleSensor.nextSample();
        amostrasColetadas++;
      } else {
        // Se não disponível, aguarda um pouco e tenta novamente
        delay(5);
        particleSensor.check();
        if (particleSensor.available()) {
          redBuffer[i] = particleSensor.getRed();
          irBuffer[i] = particleSensor.getIR();
          particleSensor.nextSample();
          amostrasColetadas++;
        } else {
          // Se ainda não disponível, usa valor anterior ou zero
          if (i > 0) {
            redBuffer[i] = redBuffer[i-1];
            irBuffer[i] = irBuffer[i-1];
          } else {
            redBuffer[i] = 0;
            irBuffer[i] = 0;
          }
        }
      }
      // Pequeno delay entre amostras para dar tempo ao sensor
      delay(2);
      // Alimenta watchdog a cada 20 amostras (menos frequente para não interferir)
      if (i % 20 == 0) {
        yield();
      }
    }
    
    // Se não coletou amostras suficientes, não calcula
    if (amostrasColetadas < MAX_SPO2_BUFFER / 2) {
      spo2Atual = 0;
      Serial.println("[SENSOR] ⚠️ Amostras insuficientes");
      tft.setCursor(10, 45);
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.print("Amostras insuficientes");
      return;
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
      Serial.println("[SENSOR] ⚠️ Medição inválida - dedo em movimento?");

      // Atualizar display
      tft.setCursor(10, 45);
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.print("Medicao Invalida");
      tft.fillRect(0, 80, tft.width(), 30, TFT_BLACK);
    }
    
  } else {
    // 5. DEDO AUSENTE
    spo2Atual = 0;
    Serial.println("[SENSOR] ⚠️ Dedo ausente - coloque o dedo no sensor");
    
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
  pinMode(VIBRA_PIN, OUTPUT);
  digitalWrite(VIBRA_PIN, LOW);
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

  // Inicializar sensor MAX30105 (sem travar se não encontrar)
  sensorDisponivel = particleSensor.begin(Wire, I2C_SPEED_STANDARD);
  if (!sensorDisponivel) {
    Serial.println("[SENSOR] ⚠️ Sensor MAX30105 não encontrado - continuando sem sensor");
    tft.fillRect(0, 70, tft.width(), 100, TFT_BLACK);
    tft.setCursor(10, 75);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.print("Sensor OFFLINE - Continuando...");
  } else {
    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x1F);
    particleSensor.setPulseAmplitudeIR(0x1F);
    Serial.println("[SENSOR] ✅ Sensor MAX30105 inicializado com sucesso");
  }
  
  // Inicialização do ESP-NOW (será configurado a cada ciclo após conectar WiFi)
  // IMPORTANTE: ESP-NOW precisa que WiFi esteja inicializado (mesmo que desconectado)
  WiFi.mode(WIFI_STA); // Garante modo STA para ESP-NOW funcionar
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("[NOW] ❌ Erro ao iniciar ESP-NOW");
    Serial.println("[NOW] Tentando reinicializar...");
    esp_wifi_deinit();
    delay(100);
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
      Serial.println("[NOW] ❌ Erro crítico ao iniciar ESP-NOW");
      while (true) {
        delay(1000);
        yield();
      }
    }
  }

  Serial.println("[NOW] ✅ ESP-NOW inicializado");
  
  dados.id = 1;
  dados.ledOn = false; 
  
  Serial.println("\n========================================");
  Serial.println("  SISTEMA INICIALIZADO");
  Serial.println("  Ciclo: SENSOR -> WIFI -> API -> ESP-NOW");
  Serial.println("========================================");
}

// --- LOOP ---
void loop() {
  
  // ========================================
  // CICLO SEQUENCIAL - SEM TIMERS
  // ========================================
  
  // 1. LEITURA DO SENSOR
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("  CICLO INICIADO");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  lerSensorECalcularSpO2();

  // Lógica de Alerta: SpO2 <= 95 E a medição foi válida (SpO2 > 0)
  bool alerta = (spo2Atual <= 95 && spo2Atual > 0);
  
  // Detecta mudança de estado do alerta
  alertaAnterior = alertaAtivo;
  alertaAtivo = alerta;
  
  // Se alerta foi ativado (mudou de false para true), inicia sequência de 2 piscadas
  if (alertaAtivo && !alertaAnterior) {
    ledPiscando = true;
    contadorPiscadas = 0;
    ultimoPiscarLED = millis();
    digitalWrite(LED_PIN, HIGH); // Primeira piscada começa ligado
    contadorPiscadas++;
  }

  // Log organizado da leitura
  if (spo2Atual > 0) {
    Serial.printf("[SENSOR] 📊 Leitura: SpO2 = %d%%\n", spo2Atual);
    Serial.printf("[SENSOR]    Status: %s\n", alerta ? "🚨 ALERTA" : "✅ NORMAL");
  }

  // Atualização do display e estado local
  if (alerta) {
    digitalWrite(VIBRA_PIN, HIGH);
    Serial.println("[SENSOR] 📳 Vibrador: LIGADO");
    Serial.println("[SENSOR] 💡 LED: PISCANDO");
  } else {
    digitalWrite(VIBRA_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    ledPiscando = false;
    contadorPiscadas = 0;
    Serial.println("[SENSOR] 📳 Vibrador: DESLIGADO");
    Serial.println("[SENSOR] 💡 LED: DESLIGADO");
  }    
    
  // Atualiza status geral no TFT
  tft.fillRect(0, 110, tft.width(), 30, TFT_BLACK);
  tft.setCursor(10, 115);
  tft.setTextColor(alerta ? TFT_RED : TFT_GREEN, TFT_BLACK);
  tft.print("STATUS: " + String(alerta ? "ALERTA" : "NORMAL"));

  // Se leitura inválida, pula o ciclo
  if (spo2Atual == 0) {
    Serial.println("[SENSOR] ⚠️ Leitura inválida - pulando ciclo");
    // Delay com alimentação do watchdog
    unsigned long inicioDelay = millis();
    while (millis() - inicioDelay < 2000) {
      delay(100);
      yield(); // Alimenta watchdog
    }
    return;
  }

  // 2. CONECTAR WIFI E ENVIAR PARA API
  Serial.println("\n[WIFI] 🔌 Conectando...");
  conectarWiFi();
  
  // Configura ESP-NOW após conectar WiFi (para sincronizar canal)
  if (WiFi.status() == WL_CONNECTED && wifiChannel > 0) {
    // Remove peer anterior se existir
    esp_now_del_peer(broadcastAddress);
    
    // Adiciona peer com canal correto
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = wifiChannel;
    peerInfo.encrypt = false;
    
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("[NOW] ⚠️ Falha ao adicionar peer");
    } else {
      Serial.printf("[NOW] ✅ Peer configurado - Canal %d\n", wifiChannel);
    }
    
    executarEnvioAPI(spo2Atual);
  } else {
    Serial.println("[WIFI] ❌ Falha na conexão - pulando envio API");
    tft.fillRect(0, 150, tft.width(), 30, TFT_BLACK);
    tft.setCursor(10, 155);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("WIFI: FALHOU");
  }

  // 3. DESCONECTAR WIFI
  Serial.println("[WIFI] 🔌 Desconectando...");
  WiFi.disconnect(false); // false = não apaga credenciais, apenas desconecta
  delay(100);
  yield(); // Alimenta watchdog
  Serial.println("[WIFI] ✅ Desconectado (modo STA mantido)");

  // 4. DELAY DE 2 SEGUNDOS APÓS DESCONECTAR WIFI
  Serial.println("[WIFI] ⏳ Aguardando 2s...");
  unsigned long inicioDelay = millis();
  while (millis() - inicioDelay < DELAY_APOS_DESCONECTAR_WIFI) {
    delay(100);
    yield(); // Alimenta watchdog a cada 100ms
  }

  // 5. ENVIAR PARA RECEPTOR COM ESP-NOW
  enviarESPNow(spo2Atual);

  // 6. DELAY DE 2 SEGUNDOS APÓS ESP-NOW
  Serial.println("[NOW] ⏳ Aguardando 2s...");
  unsigned long inicioDelay2 = millis();
  while (millis() - inicioDelay2 < DELAY_APOS_ESPNOW) {
    delay(100);
    yield(); // Alimenta watchdog a cada 100ms
  }

  // 7. RECOMEÇAR CICLO
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("  CICLO COMPLETO - RECOMEÇANDO");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // Pequeno delay para evitar loop infinito
  delay(100);
}