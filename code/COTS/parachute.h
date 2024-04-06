/* CONFIGURAÇÕES SKIBS */

#define SKIB1 13
#define SKIB2 12
#define RANGE_FALL_DETECTION 10

// Em ms
#define TIME_BETWEEN_ACTIVATIONS 8000

// Em metros
#define GAP_BETWEEN_ACTIVATIONS 150

// Em metros
#define HEIGHT_FOR_2_STAGE 500

// Em millisegundos
#define SKIB_TIME 250

bool isDropping = false;

bool parachute1Activated = false;
bool parachute2Activated = false;

double timeForStage1 = 0;
double timeForStage2 = 0;

void setupSkibPins() {
  pinMode(SKIB1, OUTPUT);
  pinMode(SKIB2, OUTPUT);
  println("Skibs configurados!");
}

void analyzeStateOfRocket() {
  if((maximumAltitudeValue - altitudeAtual) > RANGE_FALL_DETECTION) {
    isDropping = true;
  }
  if(soloData.openParachute == 1) {
    isDropping = true;
  }
}

void activateStage1() {
  digitalWrite(SKIB1, HIGH);
  println("1 Skib ativado!");

  timeForStage1 = millis();
  parachute1Activated = true;
}

void activateStage2() {
  digitalWrite(SKIB2, HIGH);
  println("2 Skib ativado!");

  timeForStage2 = millis();
  parachute2Activated = true;
}

void deactivateStage1() {
  digitalWrite(SKIB1, LOW);
  println("1 Skib desativado!");
}

void deactivateStage2() {
  digitalWrite(SKIB2, LOW);
  println("2 Skib desativado!");
}

void activateParachutes() {
  if((maximumAltitudeValue - initial_altitude) > (HEIGHT_FOR_2_STAGE + GAP_BETWEEN_ACTIVATIONS)) {
    if(parachute1Activated == false) {
      activateStage1();
    }

    if((altitudeAtual - initial_altitude) <= HEIGHT_FOR_2_STAGE) {
      activateStage2();
    }
  }

  if((maximumAltitudeValue - initial_altitude) <= HEIGHT_FOR_2_STAGE) {
    if(parachute1Activated == false) {
      activateStage1();
    }

    if(parachute1Activated && parachute2Activated == false && 
      (millis() - timeForStage1) > TIME_BETWEEN_ACTIVATIONS) {
        activateStage2();
    }
  }

  if(parachute1Activated && millis() - timeForStage1 >= SKIB_TIME) {
    deactivateStage1();
  }

  if(parachute2Activated && millis() - timeForStage2 >= SKIB_TIME) {
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
    allData.data.parachute = 1;
  }

  if(parachute2Activated) {
    allData.data.parachute = 2;
  }
}