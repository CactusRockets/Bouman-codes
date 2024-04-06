#include <SPI.h>
#include <Wire.h>

#define BUTTON_PIN 4

int valueButton;

// O tamanho deste pacote não deve exceder 32 bytes
struct PacketData {
  float time;
  float altitude_MPU;
  float temperature;
  float pressure;
  float altitude;
  float variation_altitude;
  float acceleration_Z;
  int parachute; 
};

struct PacketGPSData {
  long latitude, longitude;
};

struct AllPacketData {
  PacketData data;
  PacketGPSData GPSData;
};

struct SoloData {
  int parachute; 
};

String soloMessage = "";
String AllDados = "";

AllPacketData allData;
SoloData soloData;

#include "telemetry.h"

void clearData() {
  allData = {
    { 0,0,0,0,0,0,0,0 },
    { 0,0 }
  };
  soloData = { 0 };
}

void printData() {
  if(LORA_WAY == LORA_STRING_METHOD) {
    Serial.println(AllDados);

  } else if(LORA_WAY == LORA_STRUCT_METHOD) {
    // Armazena os dados em uma string
    String dados = String(allData.data.time, 3)         
      + "," + String(allData.data.temperature, 3)       
      + "," + String(allData.data.altitude, 3)          
      + "," + String(allData.data.variation_altitude, 3)
      + "," + String(allData.data.acceleration_Z, 3)    
      + "," + String(allData.data.altitude_MPU, 3)      
      + "," + String(allData.data.pressure, 3)
      + "," + String(allData.data.parachute);     
    String GPSDados = String(allData.GPSData.latitude, 3)
      + "," + String(allData.GPSData.longitude, 3);

    AllDados = dados + "," + GPSDados;
    Serial.println(AllDados);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Serial inicializada!");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  clearData();
  setupTelemetry();
}

void loop() {
  if(LoRaSerial.available() > 0) {

    if(LORA_WAY == LORA_STRING_METHOD) {
      receiveString();

    } else if(LORA_WAY == LORA_STRUCT_METHOD) {
      receive(&allData);
      
    }
    printData();
  }

  valueButton = digitalRead(BUTTON_PIN);
  if(valueButton == LOW) {

    soloData.parachute = 1;
    soloMessage = "1";

    if(LORA_WAY == LORA_STRING_METHOD) {
      transmitString(soloMessage);

    } else if(LORA_WAY == LORA_STRUCT_METHOD) {
      transmit(&soloData);

    }
  }
}

