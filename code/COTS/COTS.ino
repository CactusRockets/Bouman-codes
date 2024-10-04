#include <SPI.h>
#include <Wire.h>
#include <HardwareSerial.h>

#define ESP_BLUE_LED 2

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

bool LED_ACESO = false;

#include "serial.h"
#include "buzzer.h"
#include "bmp.h"
#include "imu.h"
#include "parachute.h"

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
  pinMode(ESP_BLUE_LED, OUTPUT);

  initial_altitude = getAltitude();

  digitalWrite(ESP_BLUE_LED, HIGH);
  setupBuzzer();
  tripleBuzzerBip();
}

int extractNumber(String numberText, int width, bool considerSignal=false) {
  int slicePosition = 1;
  while (numberText[slicePosition] == '0' && slicePosition < width) {
    slicePosition++;
  }

  String slicedNumber = numberText.substring(slicePosition, width);

  int realNumber = slicedNumber.toInt();

  if (!considerSignal) return realNumber;
  else {
    if (numberText[0] == '+') {
      return realNumber;
    }
    if (numberText[0] == '-') {
      return -1*realNumber;
    }
  }
}

void decodeMessage(String message) {
  //int package = extractNumber(message.substring(0, 5), 5);
  float altitude = 0.001 * extractNumber(message.substring(0, 7), 7, true);
  float accel_z = 0.01 * extractNumber(message.substring(7, 12), 5, true);
  float quaternion_w = 0.01 * extractNumber(message.substring(12, 16), 4, true);
  float quaternion_x = 0.01 * extractNumber(message.substring(16, 20), 4, true);
  float quaternion_y = 0.01 * extractNumber(message.substring(20, 24), 4, true);
  float quaternion_z = 0.01 * extractNumber(message.substring(24, 28), 4, true);
  int parachute = extractNumber(message.substring(28, 29), 1);
  //float latitude = 0.001 * extractNumber(message.substring(29, 35), 6, true);
  //float longitude = 0.001 * extractNumber(message.substring(35, 41), 6, true);

  Serial.println("-------------------------------------");
  //Serial.println("Pacote: " + String(package));
  Serial.println("Altitude: " + String(altitude, 3));
  Serial.println("Aceleração em Z: " + String(accel_z, 2));
  Serial.println("Quaternion W: " + String(quaternion_w, 2)); 
  Serial.println("Quaternion X: " + String(quaternion_x, 2));
  Serial.println("Quaternion Y: " + String(quaternion_y, 2));
  Serial.println("Quaternion Z: " + String(quaternion_z, 2));
  Serial.println("Paraquedas : " + String(parachute));
  //Serial.println("Latitude: " + String(latitude, 6)); 
  //Serial.println("Longitude: " + String(longitude, 6)); 
  Serial.println("");
}

void loop() {
  getSensorsMeasures();
  saveMessage();

  if(ENABLE_BUZZER) {
    beepIntermitating();
  }

  Serial.println(cots_message);
  decodeMessage(cots_message);
  CotsSerial.println(cots_message);
  activateStage1();
  delay(500);
  activateStage2();
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