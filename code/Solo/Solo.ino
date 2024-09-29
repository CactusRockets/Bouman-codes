#include <SPI.h>
#include <Wire.h>
#include "telemetry.h"



#define BUTTON_PIN 4
#define EXPECTED_PACKET_SIZE 31
int valueButton;
String solo_message = "";




void setup() {
  Serial.begin(9600);
  Serial.println("Serial inicializada!");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  setupTelemetry();
}

void loop() {
  telemetry_message.clear();
  if(LoRaSerial.available() > 0) {

    Serial.println("LoRa available!");

    receiveString();
    Serial.println(telemetry_message);
  }
}

