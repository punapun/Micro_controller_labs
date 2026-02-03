#include <Arduino.h>

#define ledPin 27
#define buzzerPin 26
#define sensorPin 5

#define BUZZER_CH 0
#define BUZZER_FREQ 1000
#define BUZZER_RES 8

bool alarmOn = false;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT_PULLUP);

  ledcSetup(BUZZER_CH, BUZZER_FREQ, BUZZER_RES);
  ledcAttachPin(buzzerPin, BUZZER_CH);

  digitalWrite(ledPin, LOW);
  ledcWrite(BUZZER_CH, 0);
}

void loop() {
  int deteksi = digitalRead(sensorPin);

  if (deteksi == LOW && !alarmOn) {
    alarmOn = true;
    digitalWrite(ledPin, HIGH);
    ledcWrite(BUZZER_CH, 128);
    Serial.println("Benda Terdeteksi!!!");
  }

  if (deteksi == HIGH && alarmOn) {
    alarmOn = false;
    digitalWrite(ledPin, LOW);
    ledcWrite(BUZZER_CH, 0);
    Serial.println("Tidak ada Benda...");
  }

  delay(50); // debounce
}
  