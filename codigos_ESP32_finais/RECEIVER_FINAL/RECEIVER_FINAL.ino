#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h> 

#define BUZZER_PIN 13 // Pino do Buzzer
#define LAMPADA_PIN 14 // Pino da Lâmpada
#define LED_PIN 2 // Pino do LED

// Struct deve ser idêntica à do transmissor
typedef struct {
  int id;
  bool ledOn; 
} mensagem;

mensagem dados;

// Variáveis para controle do LED piscante (2 piscadas apenas)
bool alertaAtivo = false;
bool alertaAnterior = false;
bool ledPiscando = false;
int contadorPiscadas = 0;
unsigned long ultimoPiscarLED = 0;

// Callback ESP-NOW (ABORDAGEM SIMPLES DOS CÓDIGOS QUE FUNCIONAM)
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&dados, incomingData, sizeof(dados));

  // Log organizado
  Serial.println("\n========================================");
  Serial.println("[RECEPTOR] 📦 Dados recebidos via ESP-NOW");
  Serial.println("========================================");
  Serial.print("  ID da Medição: ");
  Serial.println(dados.id);
  Serial.print("  Status do Alerta: ");
  Serial.println(dados.ledOn ? "🚨 ATIVO" : "✅ NORMAL");
  Serial.println("========================================");

  // Detecta mudança de estado do alerta
  alertaAnterior = alertaAtivo;
  alertaAtivo = dados.ledOn;

  // Se alerta foi ativado (mudou de false para true), inicia sequência de 2 piscadas
  if (alertaAtivo && !alertaAnterior) {
    ledPiscando = true;
    contadorPiscadas = 0;
    ultimoPiscarLED = millis();
    digitalWrite(LED_PIN, HIGH); // Primeira piscada começa ligado
    contadorPiscadas++;
  }

  // Ativa BUZZER e LÂMPADA quando há alerta
  if (dados.ledOn) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LAMPADA_PIN, HIGH);
    Serial.println("[RECEPTOR] 🔊 PINO 13 (BUZZER): LIGADO");
    Serial.println("[RECEPTOR] 💡 PINO 14 (LÂMPADA): LIGADO");
    Serial.println("[RECEPTOR] 💡 PINO 2 (LED): PISCANDO 2 VEZES");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LAMPADA_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    ledPiscando = false;
    contadorPiscadas = 0;
    Serial.println("[RECEPTOR] 🔊 PINO 13 (BUZZER): DESLIGADO");
    Serial.println("[RECEPTOR] 💡 PINO 14 (LÂMPADA): DESLIGADO");
    Serial.println("[RECEPTOR] 💡 PINO 2 (LED): DESLIGADO");
  }
  Serial.println();
}


void setup() {
  Serial.begin(115200);
  
  // Configura pinos de saída
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LAMPADA_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Inicializa todos em LOW
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LAMPADA_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Serial.println("SLEEP HELP - 3CT");
  delay(2000);
  
  // Força WiFi em modo STA (ABORDAGEM SIMPLES DOS CÓDIGOS QUE FUNCIONAM)
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

  // Inicializa ESP-NOW (ABORDAGEM SIMPLES DOS CÓDIGOS QUE FUNCIONAM)
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  // Registra callback (ABORDAGEM SIMPLES - SEM adicionar peer)
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Pronto para receber");
}

void loop() {
  // Faz o LED piscar apenas 2 vezes quando alerta é ativado
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
  
  delay(10); // Pequeno delay para não sobrecarregar
}