#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP3XX bmp; // I2C

#define BMP_ADDRESS 0x77

float initial_altitude = 0;
int altitudeReadQuantity = 5;
int loopCounter = 0;

void setup() {
  Serial.begin(9600);
  
  // Inicia comunicação I2C
  if (!bmp.begin_I2C(BMP_ADDRESS)) {
    Serial.println("Falha ao inicializar o BMP390!");
    while (1);
  }
  
  // Configura o sensor
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  delay(5000);
}

void loop() {
  loopCounter++;

  // Atualiza as leituras do sensor
  if (!bmp.performReading()) {
    Serial.println("Falha ao realizar a leitura do sensor!");
    return;
  }

  if(loopCounter <= altitudeReadQuantity) {
    initial_altitude = bmp.readAltitude(1013.25);
  } else {
    // Exibe as leituras
    Serial.print("Temperatura = ");
    Serial.print(bmp.temperature);
    Serial.println(" *C");

    Serial.print("Pressão = ");
    Serial.print(bmp.pressure / 100.0);
    Serial.println(" hPa");

    // Calcula altitude usando a pressão no nível do mar (padrão: 1013.25 hPa)
    Serial.print("Altitude = ");
    // Padrão ao nível do mar
    Serial.print(bmp.readAltitude(1013.25) - initial_altitude);
    Serial.println(" m");
  }

  // Aguarda 2 segundos antes de atualizar
  delay(100);
}
