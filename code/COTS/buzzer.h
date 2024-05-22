/* CONFIGURAÇÕES BUZZER */

#define BUZZER_PIN 14
#define BEEP_TIME 300

void setupBuzzer() {
  pinMode(BUZZER_PIN, OUTPUT);
  println("Buzzer configurado!");
}

void activateBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH);
}

void desactivateBuzzer() {
  digitalWrite(BUZZER_PIN, LOW);
}
