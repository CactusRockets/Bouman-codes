#include <SPI.h>
#include <Wire.h>
#include "telemetry.h"

#define BUTTON_PIN 4
#define EXPECTED_PACKET_SIZE 31
int valueButton;
String solo_message = "";

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

void setup() {
  Serial.begin(9600);
  Serial.println("Serial inicializada!");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  setupTelemetry();
}

void decodeMessage(String message) {
  int package = extractNumber(message.substring(0, 5), 5);
  float altitude = 0.001 * extractNumber(message.substring(5, 12), 7, true);
  float accel_z = 0.01 * extractNumber(message.substring(12, 17), 5, true);
  float quaternion_w = 0.01 * extractNumber(message.substring(17, 21), 4, true);
  float quaternion_x = 0.01 * extractNumber(message.substring(21, 25), 4, true);
  float quaternion_y = 0.01 * extractNumber(message.substring(25, 29), 4, true);
  float quaternion_z = 0.01 * extractNumber(message.substring(29, 33), 4, true);
  int parachute = extractNumber(message.substring(33, 34), 1) == 0;
  float latitude = 0.001 * extractNumber(message.substring(34, 40), 6, true);
  float longitude = 0.001 * extractNumber(message.substring(40, 46), 6, true);

  Serial.println("-------------------------------------");
  Serial.println("Pacote: " + String(package));
  Serial.println("Altitude: " + String(altitude, 3));
  Serial.println("Aceleração em Z: " + String(accel_z, 2));
  Serial.println("Quaternion W: " + String(quaternion_w, 2)); 
  Serial.println("Quaternion X: " + String(quaternion_x, 2));
  Serial.println("Quaternion Y: " + String(quaternion_y, 2));
  Serial.println("Quaternion Z: " + String(quaternion_z, 2));
  Serial.println("Paraquedas Ativado? : " + String(parachute));
  Serial.println("Latitude: " + String(latitude, 6)); 
  Serial.println("Longitude: " + String(longitude, 6)); 
  Serial.println("");
}

void loop() {
  telemetry_message.clear();
  if(LoRaSerial.available() > 0) {

    Serial.println("LoRa available!");

    receiveString();

    // PADRÃO DE MENSAGEM TRANSMITIDA - CUIDADO AO MUDAR ISSO
    // Numero do pacote -> primeiros 5 dígitos sem sinal
    // Altitude -> próximos 7 dígitos COM sinal
    // Aceleração em Z -> próximos 5 dígitos COM sinal
    // Quaternion W -> próximos 4 dígitos COM sinal
    // Quaternion X -> próximos 4 dígitos COM sinal
    // Quaternion Y -> próximos 4 dígitos COM sinal
    // Quartenion Z -> próximos 4 dígitos COM sinal
    // CUIDADO - Paraquedas - próximo dígito, no final do quaternion Z, é uma flag de ativação do paraquedas
    // Latitude -> próximos 6 dígitos COM sinal
    // Longitude -> próximos 6 dígitos COM sinal

    decodeMessage(telemetry_message);

    // Serial.println(telemetry_message);
  }
}

