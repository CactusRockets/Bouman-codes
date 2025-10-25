#define RX_PIN 3
#define TX_PIN 1

HardwareSerial CotsSerial(0);

void setupCots() {
  CotsSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
}

void readCots() {
  Serial.println("Verificando se existem dados vindos do COTS: ");
  
  if(CotsSerial.available() > 0) {
    cots_message = CotsSerial.readStringUntil('\n');
    Serial.println(cots_message);

    while (CotsSerial.available() > 0) {
      CotsSerial.read();
    }
  }
}