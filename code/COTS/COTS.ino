#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <HardwareSerial.h>

#define ESP_BLUE_LED 2

#define RX2_PIN 16
#define TX2_PIN 17

#define ENABLE_SERIAL false
#define ENABLE_BMP true
#define ENABLE_MPU false
#define ENABLE_SKIBS false
#define ENABLE_BUZZER false

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

  // Print dos dados
  Serial.print("Initial altitude: ");
  Serial.println(initial_altitude);
  Serial.print("Altitude: ");
  Serial.println(allData.bmpData.altitude);
  Serial.print("Temperature: ");
  Serial.println(allData.bmpData.temperature);
  Serial.print("Pressure: ");
  Serial.println(allData.bmpData.pressure);
  Serial.println("");

  delay(500);
}