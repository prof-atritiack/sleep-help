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

// --- CONFIGURAÇÕES DE COMUNICAÇÃO (TIMERS OTIMIZADOS) ---
#define INTERVALO_LEITURA_SENSOR 5000  // Intervalo entre leituras do sensor (5 segundos)
#define DELAY_API_APOS_LEITURA 3000   // Envio da API ocorre 3 segundos após a leitura
#define DELAY_NOW_APOS_API 10000      // Envio ESP-NOW ocorre 10 segundos após a API
#define INTERVALO_LEITURA 10          // Pequeno delay no loop para evitar loop infinito na leitura

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
int valorRealSpO2 = 0; // Armazena o valor real da leitura
unsigned long agendarEnvioSimulado = 0; // Agenda envio do valor simulado
bool sensorDisponivel = false; // Flag para indicar se o sensor está disponível
int contadorTeste = 0; // Contador para alternar valores no modo teste
bool enviandoAPI = false; // Flag para evitar múltiplos envios simultâneos
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
  // PROTEÇÃO: Evita múltiplos envios simultâneos
  if (enviandoAPI) {
    Serial.println("[API] ⚠️ Envio já em andamento, ignorando chamada duplicada");
    return;
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    tft.fillRect(0, 150, tft.width(), 30, TFT_BLACK);
    tft.setCursor(10, 155);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("API FALHOU: SEM WIFI");
    Serial.println("[API] ❌ Wi-Fi desconectado! Pulando POST.");
    agendarEnvioAPI = 0; 
    return;
  }
  
  // Marca que está enviando
  enviandoAPI = true;
  
  Serial.println("\n========================================");
  Serial.println("[TRANSMISSOR] 📡 ENVIANDO PARA API");
  Serial.println("========================================");
  Serial.print("  SpO2: ");
  Serial.print(spo2);
  Serial.println("%");
  Serial.print("  Timestamp: ");
  Serial.println(getCurrentTime());
  Serial.println("========================================");
  
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
    Serial.println("[TRANSMISSOR] ✅ API: Envio bem-sucedido!");
    Serial.print("  Código HTTP: ");
    Serial.println(httpResponseCode);
    Serial.println("========================================\n");
    piscarLED(1, 100);
  } else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("API: FALHOU! C:");
    tft.print(httpResponseCode);
    Serial.println("[TRANSMISSOR] ❌ API: Falha no envio!");
    Serial.print("  Código HTTP: ");
    Serial.println(httpResponseCode);
    Serial.print("  Erro: ");
    Serial.println(http.errorToString(httpResponseCode));
    Serial.println("========================================\n");
    piscarLED(5, 100);
  }
  
  http.end();
  agendarEnvioAPI = 0;
  
  // Libera flag de envio
  enviandoAPI = false;
  
  // ESP-NOW: Aguarda 10 segundos e envia DIRETAMENTE (ABORDAGEM SIMPLES)
  Serial.println("[NOW] ⏰ Aguardando 10 segundos para enviar ESP-NOW...");
  delay(DELAY_NOW_APOS_API); // Aguarda 10 segundos
  
  // Prepara e envia ESP-NOW DIRETAMENTE (ABORDAGEM SIMPLES - igual aos códigos que funcionam)
  dados.ledOn = (spo2 <= 95 && spo2 > 0);
  dados.id = contadorMedicoes;
  
  Serial.println("\n========================================");
  Serial.println("[TRANSMISSOR] 📡 ENVIANDO VIA ESP-NOW");
  Serial.println("========================================");
  Serial.print("  ID da Medição: ");
  Serial.println(dados.id);
  Serial.print("  SpO2: ");
  Serial.print(spo2);
  Serial.println("%");
  Serial.print("  Status do Alerta: ");
  Serial.println(dados.ledOn ? "🚨 ATIVO" : "✅ NORMAL");
  
  esp_now_send(broadcastAddress, (uint8_t *)&dados, sizeof(dados));
  
  Serial.println("[TRANSMISSOR] ✅ Dados enviados via ESP-NOW");
  Serial.println("========================================\n");
  
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
      
      Serial.printf("[TESTE] Medição simulada #%d - SpO2: %d%%\n", contadorMedicoes, spo2Atual);
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
  
  // 1. CHECA SE O DEDO ESTÁ PRESENTE (sem loops, apenas verifica se disponível)
  particleSensor.check(); // Atualiza estado do sensor
  
  long irAvg = 0;
  int nTest = 5;
  int amostrasValidas = 0;
  
  for (int i = 0; i < nTest; i++) {
    if (particleSensor.available()) {
      irAvg += particleSensor.getIR();
      particleSensor.nextSample();
      amostrasValidas++;
    }
  }
  
  // Se não coletou amostras suficientes, assume sensor offline
  if (amostrasValidas == 0) {
    spo2Atual = 0;
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
    Serial.println("[SENSOR] Dedo detectado. Coletando amostras...");
    
    // Coleta MAX_SPO2_BUFFER amostras (sem loops, apenas verifica disponibilidade)
    int amostrasColetadas = 0;
    for (int i = 0; i < MAX_SPO2_BUFFER; i++) {
      particleSensor.check(); // Atualiza estado
      if (particleSensor.available()) {
        redBuffer[i] = particleSensor.getRed();
        irBuffer[i] = particleSensor.getIR();
        particleSensor.nextSample();
        amostrasColetadas++;
      } else {
        // Se não disponível, usa zero ou valor anterior
        if (i > 0) {
          redBuffer[i] = redBuffer[i-1];
          irBuffer[i] = irBuffer[i-1];
        } else {
          redBuffer[i] = 0;
          irBuffer[i] = 0;
        }
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
    Serial.println("⚠️ Sensor MAX30102 não encontrado - continuando sem sensor");
    tft.fillRect(0, 70, tft.width(), 100, TFT_BLACK);
    tft.setCursor(10, 75);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.print("Sensor OFFLINE - Continuando...");
  } else {
    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x1F);
    particleSensor.setPulseAmplitudeIR(0x1F);
    Serial.println("[SENSOR] Sensor MAX30105 inicializado com sucesso.");
  }
  
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

  // Inicialização do ESP-NOW (ABORDAGEM SIMPLES DOS CÓDIGOS QUE FUNCIONAM)
  if (esp_now_init() != ESP_OK) {
    Serial.println("[NOW] ❌ Erro ao iniciar ESP-NOW");
    while (true);
  }

  // Registra o receptor como peer (ABORDAGEM SIMPLES)
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[NOW] ❌ Erro ao adicionar peer");
    while (true);
  }

  Serial.println("[NOW] ✅ ESP-NOW inicializado e pronto para enviar");
  Serial.printf("[NOW] Peer: %02X:%02X:%02X:%02X:%02X:%02X\n",
                broadcastAddress[0], broadcastAddress[1], broadcastAddress[2],
                broadcastAddress[3], broadcastAddress[4], broadcastAddress[5]);
  
  dados.id = 1;
  dados.ledOn = false; 
  
  Serial.println("========================================");
  Serial.println("  SISTEMA PRONTO: Leitura -> NOW -> API");
  Serial.println("========================================");
}

// --- LOOP ---
void loop() {
  
  // 1. CHECA E EXECUTA O CICLO DE LEITURA (a cada 5 segundos)
  if (millis() - ultimoEnvioNOW >= INTERVALO_LEITURA_SENSOR) {
    
    // EXECUTA A LEITURA E CÁLCULO REAL DO SPO2
    lerSensorECalcularSpO2();

    // Salva o valor real
    valorRealSpO2 = spo2Atual;

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
    Serial.println("\n========================================");
    Serial.println("[TRANSMISSOR] 📊 LEITURA DO SENSOR");
    Serial.println("========================================");
    Serial.print("  SpO2: ");
    Serial.print(spo2Atual);
    Serial.println("%");
    Serial.print("  Status: ");
    Serial.println(alerta ? "🚨 ALERTA (SpO2 <= 95%)" : "✅ NORMAL");
    Serial.println("========================================");

    // Agenda envio da API com valor REAL (apenas se não houver agendamento pendente E não estiver enviando)
    // O ESP-NOW será enviado 10 segundos APÓS o envio da API
    if (spo2Atual > 0 && agendarEnvioAPI == 0 && !enviandoAPI) {
      agendarEnvioAPI = millis() + DELAY_API_APOS_LEITURA;
      Serial.printf("[TRANSMISSOR] 📡 API agendada para envio em %lu ms\n", agendarEnvioAPI);
      // DESABILITADO: Envio simulado estava causando múltiplos envios
      // agendarEnvioSimulado = millis() + DELAY_API_APOS_LEITURA + 2000;
    } else if (spo2Atual == 0) {
      // Não envia dados inválidos
      Serial.println("[TRANSMISSOR] ⚠️ Leitura inválida, pulando envio");
    } else if (enviandoAPI) {
      Serial.println("[TRANSMISSOR] ⏳ API já está sendo enviada, aguardando...");
    } else {
      Serial.println("[TRANSMISSOR] ⏳ API já agendada, aguardando envio anterior...");
    }

    // --- ATUALIZAÇÃO DO DISPLAY E ESTADO LOCAL ---
    // Ativa VIBRA quando há alerta (LED pisca 2 vezes no loop)
    if (alerta) {
      digitalWrite(VIBRA_PIN, HIGH);
      Serial.println("[TRANSMISSOR] 📳 PINO 4 (VIBRA): LIGADO");
      Serial.println("[TRANSMISSOR] 💡 PINO 2 (LED): PISCANDO 2 VEZES");
    } else {
      digitalWrite(VIBRA_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
      ledPiscando = false;
      contadorPiscadas = 0;
      Serial.println("[TRANSMISSOR] 📳 PINO 4 (VIBRA): DESLIGADO");
      Serial.println("[TRANSMISSOR] 💡 PINO 2 (LED): DESLIGADO");
    }    
    
    // Atualiza status geral no TFT
    tft.fillRect(0, 110, tft.width(), 30, TFT_BLACK);
    tft.setCursor(10, 115);
    tft.setTextColor(alerta ? TFT_RED : TFT_GREEN, TFT_BLACK);
    tft.print("STATUS: " + String(alerta ? "ALERTA" : "NORMAL"));
    
    ultimoEnvioNOW = millis();
  }

  // 2. CHECA E EXECUTA o ENVIO DA API com valor REAL (Sequencial)
  if (agendarEnvioAPI != 0 && millis() >= agendarEnvioAPI) {
    executarEnvioAPI(spo2Atual); // Envia valor REAL para API
    agendarEnvioAPI = 0; // Limpa flag
  }

  // 3. FAZ O LED PISCAR APENAS 2 VEZES quando alerta é ativado
  if (ledPiscando && contadorPiscadas < 4) { // 4 transições = 2 piscadas completas (LIGADO->DESLIGADO->LIGADO->DESLIGADO)
    if (millis() - ultimoPiscarLED >= 200) { // Pisca a cada 200ms
      digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Alterna estado
      contadorPiscadas++;
      ultimoPiscarLED = millis();
      
      // Após 2 piscadas completas, para e deixa desligado
      if (contadorPiscadas >= 4) {
        digitalWrite(LED_PIN, LOW);
        ledPiscando = false;
      }
    }
  }

  
  // Pequeno delay para evitar loop infinito na leitura, mantendo responsividade
  delay(INTERVALO_LEITURA); 
}