#define SKIB1 13
#define SKIB2 12
#define RANGE_FALL_DETECTION 1

// Em ms
#define TIME_BETWEEN_ACTIVATIONS 8000

// Em metros
#define GAP_BETWEEN_ACTIVATIONS 2

// Em metros
#define HEIGHT_FOR_2_STAGE 1

// Em millisegundos
#define SKIB_TIME 1000

bool isDropping = false;

bool parachute1Activated = false;
bool parachute2Activated = false;

double timeForStage1 = 0;
double timeForStage2 = 0;

void setupSkibPins() {
  pinMode(SKIB1, OUTPUT);
  pinMode(SKIB2, OUTPUT);
  Serial.println("Skibs configurados!");
}

void analyzeStateOfRocket() {
  if((maximumAltitudeValue - altitudeAtual) > RANGE_FALL_DETECTION) {
    isDropping = true;
  }
}

void activateStage1() {
  digitalWrite(SKIB1, HIGH);
  Serial.println("1 Skib ativado!");

  timeForStage1 = millis();
  parachute1Activated = true;
  activateBuzzer();
}

void activateStage2() {
  digitalWrite(SKIB2, HIGH);
  Serial.println("2 Skib ativado!");

  timeForStage2 = millis();
  parachute2Activated = true;
  activateBuzzer();
}

void deactivateStage1() {
  digitalWrite(SKIB1, LOW);
  Serial.println("1 Skib desativado!");
  desactivateBuzzer();
}

void deactivateStage2() {
  digitalWrite(SKIB2, LOW);
  Serial.println("2 Skib desativado!");
  desactivateBuzzer();
}

void activateParachutes() {
  bool activate_by_height = true;

  if ((maximumAltitudeValue - altitudeAtual) > RANGE_FALL_DETECTION) {
    if(parachute1Activated == false) {
      activateStage1();
    }

    activate_by_height = (maximumAltitudeValue > HEIGHT_FOR_2_STAGE);
  }

  if (activate_by_height){
    if(altitudeAtual <= HEIGHT_FOR_2_STAGE) {
      activateStage2();
    }
  } else {
    if(
      parachute1Activated 
      && parachute2Activated == false 
      && (millis() - timeForStage1) > TIME_BETWEEN_ACTIVATIONS
    ) {
        activateStage2();
    }
  }

  if(parachute1Activated && (millis() - timeForStage1) >= SKIB_TIME) {
    deactivateStage1();
  }

  if(parachute2Activated && (millis() - timeForStage2) >= SKIB_TIME) {
    deactivateStage2();
  }
}

void checkApogee() {
  analyzeStateOfRocket();

  if(ENABLE_SKIBS) {
    if(isDropping) {
      activateParachutes();
    }
  }

  if(parachute1Activated) {
    allData.parachute = 1;
  }

  if(parachute2Activated) {
    allData.parachute = 2;
  }
}

void testActivations(int millisStage1, int millisStage2) {
  Serial.println(millis());
  if(millis() > millisStage1 && parachute1Activated == false) {
    activateStage1();
    delay(SKIB_TIME);
    deactivateStage1();
  } else if(millis() > millisStage2  && parachute2Activated == false) {
    activateStage2();
    delay(SKIB_TIME);
    deactivateStage2();
  }
}
