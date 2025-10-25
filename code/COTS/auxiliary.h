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
  // int package = extractNumber(message.substring(0, 5), 5);
  float altitude = 0.001 * extractNumber(message.substring(0, 7), 7, true);
  float accel_z = 0.01 * extractNumber(message.substring(7, 12), 5, true);
  float quaternion_w = 0.01 * extractNumber(message.substring(12, 16), 4, true);
  float quaternion_x = 0.01 * extractNumber(message.substring(16, 20), 4, true);
  float quaternion_y = 0.01 * extractNumber(message.substring(20, 24), 4, true);
  float quaternion_z = 0.01 * extractNumber(message.substring(24, 28), 4, true);
  int parachute = extractNumber(message.substring(28, 29), 1);
  // float latitude = 0.001 * extractNumber(message.substring(29, 35), 6, true);
  // float longitude = 0.001 * extractNumber(message.substring(35, 41), 6, true);

  // if (DEBUG) Serial.println("-------------------------------------");
  // if (DEBUG) Serial.println("Pacote: " + String(package));
  // if (DEBUG) Serial.println("Altitude: " + String(altitude, 3));
  // if (DEBUG) Serial.println("Aceleração em Z: " + String(accel_z, 2));
  // if (DEBUG) Serial.println("Quaternion W: " + String(quaternion_w, 2)); 
  // if (DEBUG) Serial.println("Quaternion X: " + String(quaternion_x, 2));
  // if (DEBUG) Serial.println("Quaternion Y: " + String(quaternion_y, 2));
  // if (DEBUG) Serial.println("Quaternion Z: " + String(quaternion_z, 2));
  // if (DEBUG) Serial.println("Paraquedas : " + String(parachute));
  // if (DEBUG) Serial.println("Latitude: " + String(latitude, 6)); 
  // if (DEBUG) Serial.println("Longitude: " + String(longitude, 6)); 
  // if (DEBUG) Serial.println("");
}

void printData() {
  if (DEBUG) Serial.print("Initial altitude: ");
  if (DEBUG) Serial.println(initialAltitude);
  if (DEBUG) Serial.print("Altitude: ");
  if (DEBUG) Serial.println(allData.bmpData.altitude);
  if (DEBUG) Serial.print("Temperature: ");
  if (DEBUG) Serial.println(allData.bmpData.temperature);
  if (DEBUG) Serial.print("Pressure: ");
  if (DEBUG) Serial.println(allData.bmpData.pressure);
  if (DEBUG) Serial.println("Aceleração em Z");
  if (DEBUG) Serial.println(allData.imuData.accelZ);
  if (DEBUG) Serial.println("Quaternion W");
  if (DEBUG) Serial.println(allData.imuData.quaternion_w);
  if (DEBUG) Serial.println("Quaternion X");
  if (DEBUG) Serial.println(allData.imuData.quaternion_x);
  if (DEBUG) Serial.println("Quaternion Y");
  if (DEBUG) Serial.println(allData.imuData.quaternion_y);
  if (DEBUG) Serial.println("Quaternion Z");
  if (DEBUG) Serial.println(allData.imuData.quaternion_z);
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

  if (DEBUG) Serial.println(cots_message);
}

// void beepIntermitating() {
//   if(millis() - beepTime >= BEEP_TIME) {
//     beepTime = millis();
//     isBeeping = !isBeeping;
    
//     if(isBeeping) {
//       activateBuzzer();
//     } else {
//       desactivateBuzzer();
//     }
//   }
// }