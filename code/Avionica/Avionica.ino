// Avionica 1Km

// import support libraries
#include <SPI.h>

#define ESP_BLUE_LED 2
#define BUZZER_PIN 

/*
  WHEN ENABLE_SERIAL IS TRUE, COTS IS DISABLED 
  BECAUSE THEY CONFLICT THE SAME SERIAL
*/
#define ENABLE_SERIAL false
#define ENABLE_SD true
#define ENABLE_TELEMETRY true
#define ENABLE_GPS true

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

String cots_message = "";
String telemetry_message = "";
String sd_message = "";
String solo_message = "";

float initial_altitude;
int package_counter = 0;

bool LED_ACESO = false;

// import external files
#include "serial.h"     // debug prints
#include "telemetry.h"  // telemetria
#include "moduleSD.h"   // armazenamento SD
#include "gps.h"        // localizacao gps
#include "cots.h"       // cots

void setupComponents();
void getSensorsMeasures();
void activateParachutes();
void resetStructs();
void checkApogee();
void saveMessages();

void setup() {
  // Reserva espaço de memoria para as mensagens, aumentando a perfomance
  sd_message.reserve(1500);
  telemetry_message.reserve(1500);

  // Inicializa a serial
  if(ENABLE_SERIAL) {
    Serial.begin(115200);
  }

  // Inicializa sensores e configura pinos
  print("Configurando componentes...");
  setupComponents();
  print("Configuração finalizada!");
  
  // Zera todos os valores
  resetStructs();

  // Liga LED do ESP32, integrado ao pino 2, como sinal que tudo iniciou corretamente
  pinMode(ESP_BLUE_LED, OUTPUT);

  print("Iniciando Sistema...");
  delay(1000);
}

void loop() {
  LED_ACESO = !LED_ACESO;
  if(LED_ACESO) {
    digitalWrite(ESP_BLUE_LED, LOW);
  } else {
    digitalWrite(ESP_BLUE_LED, HIGH);
  }
  getSensorsMeasures();

  // Armazena o tempo de execução
  allData.data.time = millis() / 1000.0;

  saveMessages();

  println(telemetry_message);

  if(ENABLE_SD) {
    writeOnSD(telemetry_message);
  }

  if(ENABLE_TELEMETRY) {
    transmit();
    if(hasSoloMessage()) {
      receive();
    }
  }

  delay(500);
}

void setupComponents() {
  setupCots();

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
  readCots();

  //Medições GPS
  if(ENABLE_GPS) {
    updateGPSData();
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

void saveMessages() {
  telemetry_message = telemetryMessage();
  sd_message = sdMessage();
  package_counter++;
}

String telemetryMessage(){
  // Padráo da Mensagem:
  // S -> Sinal (pode ser "+" ou "-")
  // P -> Numeracao do Pacote
  // A -> Altitude medida
  // C -> Acelecerao no eixo Z
  // W -> Quaternion w
  // X -> Quaternion x
  // Y -> Quaternion y
  // Z -> Quaternion z
  // P -> Estado do Paraquedas
  // T -> Latitude do GPS
  // G -> Longitude do GPS
  // Formato : PPPPPSAAAAAASCCCCSWWWSXXXSYYYSZZZPSTTTTTSGGGGG
  
  String packetString = fixNumberSize(package_counter, 5) + cots_message;
    
  String gpsString = fixNumberSize((int) (allData.gpsData.latitude*1000), 5, true)
    + fixNumberSize((int) (allData.gpsData.longitude*1000), 5, true);

  return (packetString + gpsString);
}

String sdMessage(){
  // Padráo da Mensagem:
  // S -> Sinal (pode ser "+" ou "-")
  // P -> Numeracao do Pacote
  // A -> Altitude medida
  // C -> Acelecerao no eixo Z
  // W -> Quaternion w
  // X -> Quaternion x
  // Y -> Quaternion y
  // Z -> Quaternion z
  // P -> Estado do Paraquedas
  // T -> Latitude do GPS
  // G -> Longitude do GPS
  // Formato : PPPPPSAAAAAASCCCCSWWWSXXXSYYYSZZZPSTTTTTSGGGGG
  
  String packetString = fixNumberSize(package_counter, 5) + cots_message;
    
  String gpsString = fixNumberSize((int) (allData.gpsData.latitude*1000), 5, true)
    + fixNumberSize((int) (allData.gpsData.longitude*1000), 5, true);

  return (packetString + gpsString);
}