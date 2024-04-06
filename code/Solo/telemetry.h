/* CONFIGURAÇÕES LORA */

#include <HardwareSerial.h>

// Pinos da Serial 2 do ESP32
#define RX2_PIN 16
#define TX2_PIN 17
#define M0 33
#define M1 32

#define LORA_STRING_METHOD 1
#define LORA_STRUCT_METHOD 2

#define LORA_WAY 1

// Usando a Serial 2 do ESP32
HardwareSerial LoRaSerial(2);
int sizeAllData = sizeof(allData);
int sizeSoloData = sizeof(soloData);

void setupTelemetry() {

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  //Configuração inicial do LoRa
  LoRaSerial.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN);
  while(!LoRaSerial);

  Serial.println("LoRa conectado!");
}

void modoReceptor() {
  // Configurações para modo Receptor
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
}

void modoTransmissor() {
  // Configurações para modo Transmissor
  digitalWrite(M0, HIGH);
  digitalWrite(M1, LOW);
}

void transmit(const SoloData* soloData) {
  modoTransmissor();
  LoRaSerial.write((const char*)soloData, sizeSoloData);
  Serial.println("\n\n");
}

void receive(AllPacketData *allData) {
  modoReceptor();
  LoRaSerial.readBytes((char*)allData, sizeAllData);
}

void transmitString(String string) {
  modoTransmissor();
  LoRaSerial.println(string);
}

void receiveString() {
  modoReceptor();
  AllDados = LoRaSerial.readStringUntil('\n');
}
