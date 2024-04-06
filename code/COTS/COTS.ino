#include <SPI.h>
#include <Wire.h>

#define ENABLE_SERIAL true
#define ENABLE_BMP true
#define ENABLE_MPU true
#define ENABLE_SKIBS false

void setup() {
  // Inicializa biblioteca I2C
  Wire.begin();
  Wire.setClock(400000);

}

void loop() {

}