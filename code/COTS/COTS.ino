#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <HardwareSerial.h>

#define ESP_BLUE_LED 2

#define RX2_PIN 16
#define TX2_PIN 17

#define ENABLE_BMP true
#define ENABLE_MPU true
#define ENABLE_SKIBS true
#define ENABLE_BUZZER true

struct BmpData {
  float temperature;
  float pressure;
  float altitude;
};

struct ImuData {
  float accelX;
  float accelY;
  float accelZ;
  float quaternion_w;
  float quaternion_x;
  float quaternion_y;
  float quaternion_z;
};

struct PacketData {
  int parachute;
  BmpData bmpData;
  ImuData imuData;
};

PacketData allData;
String cots_message = "";
float beepTime = 0;
bool isBeeping = false;
bool LED_ACESO = false;
HardwareSerial CotsSerial(2);

#include "buzzer.h"
#include "bmp.h"
#include "imu.h"
#include "parachute.h"
#include "setups.h"
#include "auxiliary.h"

void setup() {

  Wire.begin();
  Wire.setClock(400000);

  Serial.begin(9600);
  CotsSerial.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN);

  pinMode(ESP_BLUE_LED, OUTPUT);

  resetStructs();
  setupComponents();

  getInitialAltitude();
  digitalWrite(ESP_BLUE_LED, HIGH);
  tripleBuzzerBip();
}

void loop() {
  getSensorsMeasures();
  saveMessage();
  decodeMessage(cots_message);
  CotsSerial.println(cots_message);

  checkApogee();

  printData();

  delay(500);
}