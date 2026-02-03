#include <Arduino.h>

int sensorPin = 2;
int buzzerPin = 11;
int ledPin = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(sensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);

}

void loop() {

  int deteksi = digitalRead(sensorPin);

  if (deteksi == LOW) {
    tone(buzzerPin, 1000);
    digitalWrite(ledPin, HIGH);
    Serial.println("Benda Terdeteksi!!!");
  } else {
    Serial.println ("Tidak ada Benda...");
    noTone(buzzerPin);
    digitalWrite(ledPin, LOW);
  } 

  // put your main code here, to run repeatedly:
}
