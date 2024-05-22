#include <HardwareSerial.h>

#define RX_PIN 3
#define TX_PIN 1

HardwareSerial CotsSerial(0);

void setupCots() {
  CotsSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
}

void readCots() {
  if(CotsSerial.available() > 1) {
    cots_message = CotsSerial.readStringUntil('\n');
  }
}