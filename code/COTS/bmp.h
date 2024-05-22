#include <Adafruit_BMP3XX.h>
#include <Adafruit_Sensor.h>

/* CONFIGURAÇÕES BMP */
#define BMP_ADDRESS 0x77

#define SEA_LOCAL_PRESSURE 1019.0

Adafruit_BMP3XX bmp;

float maximumAltitudeValue = 0;
float altitudeAtual = 0;

void updateBMP() {
  if (!bmp.performReading()) {
    println("Failed to read BMP");
    return;
  }
}

void setupBMP() {
  if(!bmp.begin_I2C(BMP_ADDRESS)) {
    println("Failed to find BMP390 chip");
    while(1);
  }
  println("BMP390 conectado!");

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_31);
  bmp.setOutputDataRate(BMP3_ODR_0_1_HZ);

  delay(500);
  updateBMP();
}

double getAltitude() {
  return bmp.readAltitude(SEA_LOCAL_PRESSURE);
}

void readBMP() {
  updateBMP();

  allData.bmpData.temperature = bmp.temperature;
  allData.bmpData.pressure = bmp.pressure;
  allData.bmpData.altitude = getAltitude() - initial_altitude;

  // Atualiza a altura máxima
  if(allData.bmpData.altitude > maximumAltitudeValue) {
    maximumAltitudeValue = allData.bmpData.altitude;
  }
}

