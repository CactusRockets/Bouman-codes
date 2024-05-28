#include <HardwareSerial.h>
#if ENABLE_SERIAL == FALSE
#define RX_PIN 3
#define TX_PIN 1

HardwareSerial CotsSerial(0);
#endif

void setupCots() {
  #if ENABLE_SERIAL == FALSE
  CotsSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  #endif
}

void readCots() {
  #if ENABLE_SERIAL == FALSE
  if(CotsSerial.available() > 1) {
    cots_message = CotsSerial.readStringUntil('\n');
  }
  #endif
}