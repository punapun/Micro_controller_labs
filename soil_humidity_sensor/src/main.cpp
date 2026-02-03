#include <Arduino.h>

#define soilPin A0

#define ledPin1 8
#define ledPin2 9
#define ledPin3 10

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin1,OUTPUT);
  pinMode(ledPin2,OUTPUT);
  pinMode(ledPin3,OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int soil_read = analogRead(soilPin);

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);

  if (soil_read > 700) {
    digitalWrite(ledPin1, HIGH);
    Serial.println("Level 3 : Kering");
  } else if (soil_read > 400) {
    digitalWrite(ledPin2, HIGH);
    Serial.println("Level 2 : Lembab");
  } else {
    digitalWrite(ledPin3, HIGH);
    Serial.println("Level 1 : Basah");
  }
  
  Serial.print("Nilai sensor: ");
  Serial.println(soil_read);

  delay(1000);

}

