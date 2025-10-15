#define BUZZER_PIN 26
#define BEEP_TIME 100

void setupBuzzer() {
  pinMode(BUZZER_PIN, OUTPUT);
  if (DEBUG) Serial.println("Buzzer configurado!");
}

void activateBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH);
  if (DEBUG) Serial.println("Buzzer ativado!");
}

void desactivateBuzzer() {
  digitalWrite(BUZZER_PIN, LOW);
  if (DEBUG) Serial.println("Buzzer desativado!");
}

void tripleBuzzerBip() {
  activateBuzzer();
  delay(BEEP_TIME);
  desactivateBuzzer();
  delay(BEEP_TIME);

  activateBuzzer();
  delay(BEEP_TIME);
  desactivateBuzzer();
  delay(BEEP_TIME);
  
  activateBuzzer();
  delay(BEEP_TIME);
  desactivateBuzzer();
  delay(BEEP_TIME);
}

