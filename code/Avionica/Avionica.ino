// Avionica 1Km

// import support libraries
#include <SPI.h>

#define ENABLE_SERIAL true
#define ENABLE_BUZZER false
#define ENABLE_SD false
#define ENABLE_TELEMETRY false
#define ENABLE_GPS false

#define ALAVANCA 26
#define ALAVANCA_BEEP_TIME 300

struct AvionicData {
  float time;
  int parachute;
};

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

struct GpsData {
  double latitude, longitude;
};

struct PacketData {
  AvionicData data;
  BmpData bmpData;
  ImuData imuData;
  GpsData gpsData;
};

struct SoloData {
  int openParachute;
};

PacketData allData;
SoloData soloData;

String telemetry_message = "";
String sd_message = "";
String solo_message = "";

double alavancaTime = 0;
bool isBeeping = false;
bool alavancaAcionada = false;

float initial_altitude;

// import external files
#include "serial.h"     // debug prints
#include "buzzer.h"     // sinal sonoro
#include "telemetry.h"  // telemetria
#include "moduleSD.h"   // armazenamento SD
#include "gps.h"        // localizacao gps

void setupComponents();
void getSensorsMeasures();
void beepIntermitating();
void activateParachutes();
void resetStructs();
void checkApogee();
void saveMessages();

void setup() {
  // Reserva espaço de memoria para as mensagens, aumentando a perfomance
  sd_message.reserve(1500);
  telemetry_message.reserve(1500);

  // Inicializa a serial
  Serial.begin(115200);

  // Inicializa sensores e configura pinos
  setupComponents();

  // Zera todos os valores
  resetStructs();

  delay(1000);
}

void loop() {
  if(digitalRead(ALAVANCA) == HIGH) {
    if(alavancaAcionada == false) {
      // initial_altitude = getAltitude();
      alavancaAcionada = true;
    }

    getSensorsMeasures();

    beepIntermitating();

    // Armazena o tempo de execução
    allData.data.time = millis() / 1000.0;

    saveMessages();

    println(telemetry_message);

    if(ENABLE_SD) {
      writeOnSD(sd_message);
    }

    if(ENABLE_TELEMETRY) {
      transmit();
      if(hasSoloMessage()) {
        receive();
      }
    }

    delay(500);
  }
}

void setupComponents() {
  pinMode(ALAVANCA, INPUT);

  if(ENABLE_BUZZER) {
    setupBuzzer();
  }

  if(ENABLE_TELEMETRY) {
    setupTelemetry();
  }

  if(ENABLE_SD) {
    setupSd();
  }

  if(ENABLE_GPS) {
    setupGPS();
  }
}

void getSensorsMeasures() {
  //Medições GPS
  if(ENABLE_GPS) {
    updateGPSData();
  }
}

void beepIntermitating() {
  if(millis() - alavancaTime >= ALAVANCA_BEEP_TIME) {
    alavancaTime = millis();
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
    { 0,0 },        // time, parachute
    { 0,0,0 },      // temperature, pressure, altitude
    { 0,0,0 },      // accX, accY, accZ
    { 0,0 }         // latitude, longitude
  };
  soloData = { 0 }; // openParachute
}

void saveMessages() {
  String packetString = String(allData.data.time, 3)
    + "," + String(allData.bmpData.temperature, 2)
    + "," + String(allData.bmpData.pressure, 2)
    + "," + String(allData.bmpData.altitude, 3)
    + "," + String(allData.imuData.accelZ)
    + "," + String(allData.data.parachute);
    
  String gpsString = String(allData.gpsData.latitude, 3)
    + "," + String(allData.gpsData.longitude, 3);

  telemetry_message = packetString + ',' + gpsString;

  sd_message = telemetry_message;
}