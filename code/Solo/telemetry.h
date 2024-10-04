/* CONFIGURAÇÕES LORA */

#include <HardwareSerial.h>

// Pinos da Serial 2 do ESP32
#define RX2_PIN 16
#define TX2_PIN 17
#define M0 21
#define M1 22

// Usando a Serial 2 do ESP32
HardwareSerial LoRaSerial(2);

String telemetry_message = "";

void modoReceptor()
{
  // Configurações para modo Receptor
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
}

void modoTransmissor()
{
  // Configurações para modo Transmissor
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
}

void setupTelemetry()
{

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  modoReceptor();

  // Configuração inicial do LoRa
  LoRaSerial.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN);
  while (!LoRaSerial)
    ;

  Serial.println("LoRa conectado!");
}

void transmitString(String string)
{
  modoTransmissor();
  LoRaSerial.println(string);
}

void receiveString()
{
  modoReceptor();
  telemetry_message = LoRaSerial.readStringUntil('\n');
}
