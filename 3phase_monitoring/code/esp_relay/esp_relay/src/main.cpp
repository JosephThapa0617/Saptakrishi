#include <Arduino.h>

void setup() {
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
  
}

void loop() {
  digitalWrite(2,LOW);
  delay(3000);
  digitalWrite(2,HIGH);
  delay(2000);

}

