#include <SPI.h>
#include <Wire.h>
#include <HardwareSerial.h>

#define ENABLE_SERIAL true
#define ENABLE_BMP true
#define ENABLE_MPU true
#define ENABLE_SKIBS false
#define ENABLE_BUZZER false

#define RX2_PIN 16
#define TX2_PIN 17

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
float initial_altitude = 0;

float beepTime = 0;
bool isBeeping = false;

#include "serial.h"
#include "bmp.h"
#include "imu.h"
#include "parachute.h"
#include "buzzer.h"

HardwareSerial CotsSerial(2);

void beepIntermitating();
void resetStructs();

void setup() {
  // Inicializa biblioteca I2C
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000);

  CotsSerial.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN);

  resetStructs();
  setupComponents();

  initial_altitude = getAltitude();
}

void loop() {
  getSensorsMeasures();
  saveMessage();

  if(ENABLE_BUZZER) {
    beepIntermitating();
  }

  Serial.println(cots_message);
  CotsSerial.println(cots_message);
}

void setupComponents() {
  if(ENABLE_SKIBS) {
    setupSkibPins();
  }

  if(ENABLE_BUZZER) {
    setupBuzzer();
  }

  if(ENABLE_BMP) {
    setupBMP();
  }

  if(ENABLE_MPU) {
    setupMPU();
  }
}

void getSensorsMeasures() {
  // Medições BMP390
  if(ENABLE_BMP) {
    readBMP();
  }

  // Medições MPU6050
  if(ENABLE_MPU) {
    readMPU();
  }
}

String fixNumberSize(int num, int width, bool enableSignal=false){
  int numPositive = (num >= 0 ? num : -num);
  String formattedString = String(numPositive);

  while (formattedString.length() < width){
    formattedString = "0" + formattedString;
  }

  if(!enableSignal) return formattedString;

  if(num < 0) {
    formattedString = "-" + formattedString;
  } else {
    formattedString = "+" + formattedString;
  }

  return formattedString;
}

void saveMessage() {
  cots_message = fixNumberSize((int) (allData.bmpData.altitude*1000), 6, true)
    + fixNumberSize((int) (allData.imuData.accelZ*100), 4, true)
    + fixNumberSize((int) (allData.imuData.quaternion_w*100), 3, true)
    + fixNumberSize((int) (allData.imuData.quaternion_x*100), 3, true)
    + fixNumberSize((int) (allData.imuData.quaternion_y*100), 3, true)
    + fixNumberSize((int) (allData.imuData.quaternion_z*100), 3, true)
    + String(allData.parachute);
}

void beepIntermitating() {
  if(millis() - beepTime >= BEEP_TIME) {
    beepTime = millis();
    isBeeping = !isBeeping;
    
    if(isBeeping) {
      activateBuzzer();
    } else {
      desactivateBuzzer();
    }
  }
}

void resetStructs() {
  allData = {
    0,                    // parachute
    { 0,0,0 },            // temperature, pressure, altitude
    { 0,0,0,0,0,0,0 },    // accX, accY, accZ, qw, qx, qy, qz
  };
}