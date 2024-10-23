#define SKIB1 13 
#define SKIB2 12 
#define SAFE_MARGIN_ALTITUDE_ERROR 1
 
// Em ms 
#define TIME_BETWEEN_ACTIVATIONS 8000 
 
// Em metros 
#define HEIGHT_FOR_2_STAGE 3 
 
// Em millisegundos 
#define SKIB_TIME 1000 
 
bool isDropping = false; 
bool enoughHeight = true; 
 
bool parachute1Activated = false; 
bool parachute2Activated = false; 
 
double timeForStage1 = 0; 
double timeForStage2 = 0; 
 
bool alreadyDesactivatedBuzzer1 = false; 
bool alreadyDesactivatedBuzzer2 = false; 
 
void setupSkibPins() { 
  pinMode(SKIB1, OUTPUT); 
  pinMode(SKIB2, OUTPUT); 
  Serial.println("Skibs configurados!"); 
} 
 
void activateStage1() { 
  digitalWrite(SKIB1, HIGH); 
  Serial.println("1 Skib ativado!"); 
 
  timeForStage1 = millis(); 
  parachute1Activated = true; 
  if (allData.parachute < 1) allData.parachute = 1; 
 
  activateBuzzer(); 
} 
 
void activateStage2() { 
  digitalWrite(SKIB2, HIGH); 
  Serial.println("2 Skib ativado!"); 
 
  timeForStage2 = millis(); 
  parachute2Activated = true; 
  if (allData.parachute < 2) allData.parachute = 2; 
 
  activateBuzzer(); 
   
  Serial.println("Buzzer 2 Ativado!"); 
  Serial.println("Buzzer 2 Ativado!"); 
  Serial.println("Buzzer 2 Ativado!"); 
  Serial.println("Buzzer 2 Ativado!"); 
  Serial.println("Buzzer 2 Ativado!"); 
  Serial.println("Buzzer 2 Ativado!"); 
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
 
bool altitudeLessThan(double altitude1, double altitude2) { 
  return (altitude2 - altitude1 > SAFE_MARGIN_ALTITUDE_ERROR); 
} 
 
void activateParachutes() { 
  if (parachute1Activated == false) { 
    activateStage1(); 
    enoughHeight = altitudeLessThan(HEIGHT_FOR_2_STAGE, maximumAltitudeValue); 
  } 
 
  if (parachute2Activated == false && parachute1Activated) { 
    if (enoughHeight && altitudeLessThan(altitudeAtual, HEIGHT_FOR_2_STAGE)) { 
      activateStage2(); 
    }   
     
    if (!enoughHeight && (millis() - timeForStage1) > TIME_BETWEEN_ACTIVATIONS){ 
      activateStage2(); 
    } 
  } 
 
  if(parachute1Activated && (millis() - timeForStage1) >= SKIB_TIME && !alreadyDesactivatedBuzzer1) { 
    alreadyDesactivatedBuzzer1 = true; 
    deactivateStage1(); 
  } 
 
  if(parachute2Activated && (millis() - timeForStage2) >= SKIB_TIME && !alreadyDesactivatedBuzzer2) { 
    alreadyDesactivatedBuzzer2 = true; 
    deactivateStage2(); 
    Serial.println("Buzzer 2 Desativado!"); 
    Serial.println("Buzzer 2 Desativado!"); 
    Serial.println("Buzzer 2 Desativado!"); 
  } 
} 
 
bool checkIsDropping() { 
  return altitudeLessThan(altitudeAtual, maximumAltitudeValue); 
} 
 
void checkApogee() { 
  isDropping = checkIsDropping() || isDropping; 
 
  if (ENABLE_SKIBS && isDropping) { 
      activateParachutes(); 
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
