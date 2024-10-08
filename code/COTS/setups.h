void setupComponents() {
  if(ENABLE_SKIBS) {
    setupSkibPins();
  }

  if(ENABLE_BUZZER) {
    setupBuzzer();
  }

  if(ENABLE_BMP) {
    setupBMP();
  }

  if(ENABLE_MPU) {
    setupMPU();
  }
}

void getSensorsMeasures() {
  // Medições BMP390
  if(ENABLE_BMP) {
    readBMP();
  }

  // Medições MPU6050
  if(ENABLE_MPU) {
    readMPU();
  }
}

void resetStructs() {
  allData = {
    0,                    // parachute
    { 0,0,0 },            // temperature, pressure, altitude
    { 0,0,0,0,0,0,0 },    // accX, accY, accZ, qw, qx, qy, qz
  };
}