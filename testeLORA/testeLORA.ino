/*
 * ESP32 - Receptor LoRa Serial
 * Ouve por dados na LoRaSerial e os imprime no Monitor Serial.
 */

#include <HardwareSerial.h>

// Defina os pinos M0 e M1
#define M0_PIN 21
#define M1_PIN 22

// Defina os pinos para a Serial2 (UART2) do ESP32
// RX2 = 16, TX2 = 17 (Padrão)
#define LORA_RX_PIN 16
#define LORA_TX_PIN 17

// Baud rate para o Monitor Serial (PC)
#define MONITOR_BAUD 115200

// Baud rate para a comunicação UART com o módulo LoRa
// DEVE SER O MESMO do transmissor!
#define LORA_BAUD 9600

// Vamos usar a Serial2 do ESP32 e chamá-la de "LoRaSerial"
HardwareSerial LoRaSerial(2);

void setup() {
  // 1. Inicia o Monitor Serial (comunicação com o PC)
  Serial.begin(MONITOR_BAUD);
  while (!Serial); // Espera o monitor serial conectar
  Serial.println("--- Receptor LoRa Serial (ESP32) ---");
  Serial.println("Aguardando mensagens...");

  // 2. Configura os pinos de Modo (M0, M1)
  pinMode(M0_PIN, OUTPUT);
  pinMode(M1_PIN, OUTPUT);

  // 3. Coloca o módulo em Modo Normal (0) para transmitir/receber
  // M0 = LOW, M1 = LOW
  digitalWrite(M0_PIN, LOW);
  digitalWrite(M1_PIN, LOW);
  Serial.println("Módulo LoRa em Modo Normal (M0=LOW, M1=LOW)");

  // 4. Inicia a LoRaSerial (UART2) nos pinos definidos
  LoRaSerial.begin(LORA_BAUD, SERIAL_8N1, LORA_RX_PIN, LORA_TX_PIN);
  Serial.println("Serial LoRa inicializada.");
}

void loop() {
  // Verifica se há algum dado disponível no buffer da LoRaSerial
  if (LoRaSerial.available() > 0) {
    
    // Lê a string inteira até encontrar o caractere de nova linha '\n'
    // (Enviado pelo transmissor usando println)
    String receivedMessage = LoRaSerial.readStringUntil('\n');

    // Imprime a mensagem recebida no Monitor Serial
    Serial.print("Recebido: ");
    Serial.println(receivedMessage);
  }
}
