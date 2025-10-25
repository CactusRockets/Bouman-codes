#include <SD.h>

/* CONFIGURAÇÕES CARTÃO MICROSD */
#define CS_SDPIN 5

File myFile;

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

void saveDebugMessageOnSD(String message)
{
    int package = extractNumber(message.substring(0, 5), 5);
    float altitude = 0.001 * extractNumber(message.substring(5, 12), 7, true);
    float accelerationZ = 0.01 * extractNumber(message.substring(12, 17), 5, true);
    float quaternionW = 0.01 * extractNumber(message.substring(17, 21), 4, true);
    float quaternionX = 0.01 * extractNumber(message.substring(21, 25), 4, true);
    float quaternionY = 0.01 * extractNumber(message.substring(25, 29), 4, true);
    float quaternionZ = 0.01 * extractNumber(message.substring(29, 33), 4, true);
    int parachute = extractNumber(message.substring(33, 34), 1) == 0;
    bool skib1 = parachute > 0;
    bool skib2 = parachute > 1;
    float latitude = 0.001 * extractNumber(message.substring(34, 40), 6, true);
    float longitude = 0.001 * extractNumber(message.substring(40, 46), 6, true);
    float velocity = 1;
    float velocityX = 5;
    float velocityY = 10;
    float velocityZ = 10;
    float maximumVelocity = 1;
    float maximumAcceleration = 1;
    float acceleration = 1;
    float accelerationX = 1;
    float accelerationY = 5;
    float maximumAltitude = 10;

    myFile.println("Pacote: " + String(package));
    myFile.println("Altitude: " + String(altitude, 3));
    myFile.println("Aceleração em Z: " + String(accelerationZ, 2));
    myFile.println("Quaternion W: " + String(quaternionW, 2));
    myFile.println("Quaternion X: " + String(quaternionX, 2));
    myFile.println("Quaternion Y: " + String(quaternionY, 2));
    myFile.println("Quaternion Z: " + String(quaternionZ, 2));
    myFile.println("Drogo Ativado? : " + String(skib1 ? "Não" : "Sim"));
    myFile.println("Paraquedas Principal Ativado? : " + String(skib2 ? "Não" : "Sim"));
    myFile.println("Latitude: " + String(latitude, 6));
    myFile.println("Longitude: " + String(longitude, 6));
    myFile.println("");
}

void printDebugMessage(String message)
{
    int package = extractNumber(message.substring(0, 5), 5);
    float altitude = 0.001 * extractNumber(message.substring(5, 12), 7, true);
    float accelerationZ = 0.01 * extractNumber(message.substring(12, 17), 5, true);
    float quaternionW = 0.01 * extractNumber(message.substring(17, 21), 4, true);
    float quaternionX = 0.01 * extractNumber(message.substring(21, 25), 4, true);
    float quaternionY = 0.01 * extractNumber(message.substring(25, 29), 4, true);
    float quaternionZ = 0.01 * extractNumber(message.substring(29, 33), 4, true);
    int parachute = extractNumber(message.substring(33, 34), 1) == 0;
    bool skib1 = parachute > 0;
    bool skib2 = parachute > 1;
    float latitude = 0.001 * extractNumber(message.substring(34, 40), 6, true);
    float longitude = 0.001 * extractNumber(message.substring(40, 46), 6, true);
    float velocity = 1;
    float velocityX = 5;
    float velocityY = 10;
    float velocityZ = 10;
    float maximumVelocity = 1;
    float maximumAcceleration = 1;
    float acceleration = 1;
    float accelerationX = 1;
    float accelerationY = 5;
    float maximumAltitude = 10;

    Serial.println("-------------------------------------");
    Serial.println("Pacote: " + String(package));
    Serial.println("Altitude: " + String(altitude, 3));
    Serial.println("Aceleração em Z: " + String(accelerationZ, 2));
    Serial.println("Quaternion W: " + String(quaternionW, 2));
    Serial.println("Quaternion X: " + String(quaternionX, 2));
    Serial.println("Quaternion Y: " + String(quaternionY, 2));
    Serial.println("Quaternion Z: " + String(quaternionZ, 2));
    Serial.println("Drogo Ativado? : " + String(skib1 ? "Não" : "Sim"));
    Serial.println("Paraquedas Principal Ativado? : " + String(skib2 ? "Não" : "Sim"));
    Serial.println("Latitude: " + String(latitude, 6));
    Serial.println("Longitude: " + String(longitude, 6));
    Serial.println("");
}

void setupSd() {
  println("Inicializando o cartão SD...");
  
  // verifica se o cartão SD não está presente ou não pode ser inicializado
  while(!SD.begin(CS_SDPIN)) {
    println("Falha, verifique se o cartão está presente.");
  }

  // Cria arquivo data.txt e abre
  myFile = SD.open("/data.txt", FILE_APPEND);

  // Escreve dados no arquivo
  if (myFile) {
    println("Gravando dados iniciais!");
    myFile.println("Informations");
    myFile.close();
  } else {
    println("Error ao abrir data.txt");
  }
}

void writeOnSD(String str) {
  myFile = SD.open("/data.txt", FILE_APPEND);

  if (myFile) {
    println("(Dados Gravados!)");
    saveDebugMessageOnSD(str);
    myFile.close();
  } else {
    println("Error ao gravar em data.txt");
  }
}

