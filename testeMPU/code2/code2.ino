#include <Wire.h>
#include <Adafruit_Sensor.h>

#define ESP_BLUE_LED 2

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
#include "imu.h"

void resetStructs() {
  allData = {
    0,                    // parachute
    { 0,0,0 },            // temperature, pressure, altitude
    { 0,0,0,0,0,0,0 },    // accX, accY, accZ, qw, qx, qy, qz
  };
}

void setup() {
  Wire.begin();
  Wire.setClock(400000);

  Serial.begin(115200);
  pinMode(ESP_BLUE_LED, OUTPUT);

  resetStructs();
  setupMPU();

  digitalWrite(ESP_BLUE_LED, HIGH);
}

void loop() {
  readMPU();
  delay(10);
}