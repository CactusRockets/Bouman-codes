#include <HardwareSerial.h>

/* CONFIGURAÇÕES LORA */

// Pinos da Serial 2 do ESP32
#define RX2_PIN 16
#define TX2_PIN 17
#define M0 32
#define M1 33

#define LORA_STRING_METHOD 1
#define LORA_STRUCT_METHOD 2

#define LORA_WAY 1

// Usando a Serial 2 do ESP32
HardwareSerial LoRaSerial(2);
int sizeAllData = sizeof(allData);
int sizeSoloData = sizeof(soloData);

void modoReceptor() {
  // Configurações para modo Receptor
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
}

void modoTransmissor() {
  // Configurações para modo Transmissor
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
}

void setupTelemetry() {

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  modoTransmissor();

  //Configuração inicial do LoRa
  LoRaSerial.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN);
  while(!LoRaSerial);

  println("LoRa conectado!");
}

void transmitStruct(const PacketData* allData) {
  modoTransmissor();
  LoRaSerial.write((const char*)allData, sizeAllData);
  println("\n\n");
}

void receiveStruct(SoloData *soloData) {
  modoReceptor();
  LoRaSerial.readBytes((char*)soloData, sizeSoloData);
}

void transmitString(String string) {
  modoTransmissor();
  LoRaSerial.println(string);
}

void receiveString() {
  modoReceptor();
  solo_message = LoRaSerial.readStringUntil('\n');
}

void transmit() {
  if(LORA_WAY == LORA_STRING_METHOD) {
    transmitString(telemetry_message);
    // transmitString(cots_message);
  } else if(LORA_WAY == LORA_STRUCT_METHOD) {
    transmitStruct(&allData);
  }
}

void receive() {
  if(LORA_WAY == LORA_STRING_METHOD) {
    receiveString();
  } else if(LORA_WAY == LORA_STRUCT_METHOD) {
    receiveStruct(&soloData);
  }
}

bool hasSoloMessage() {
  return LoRaSerial.available() > 0;
}