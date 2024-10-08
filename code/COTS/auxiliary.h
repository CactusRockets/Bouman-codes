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

  // Serial.println("-------------------------------------");
  // Serial.println("Pacote: " + String(package));
  // Serial.println("Altitude: " + String(altitude, 3));
  // Serial.println("Aceleração em Z: " + String(accel_z, 2));
  // Serial.println("Quaternion W: " + String(quaternion_w, 2)); 
  // Serial.println("Quaternion X: " + String(quaternion_x, 2));
  // Serial.println("Quaternion Y: " + String(quaternion_y, 2));
  // Serial.println("Quaternion Z: " + String(quaternion_z, 2));
  // Serial.println("Paraquedas : " + String(parachute));
  // Serial.println("Latitude: " + String(latitude, 6)); 
  // Serial.println("Longitude: " + String(longitude, 6)); 
  // Serial.println("");
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

  Serial.println(cots_message);
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