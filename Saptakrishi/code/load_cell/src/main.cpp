#include <Arduino.h>
#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;
int gradeAcnt=0;
float total_weight=0;
int gradeAweight=100;

// LED pins
const int buzzer = 2;
HX711 scale;

void setup() {
  Serial.begin(115200);
  
  // Set up the LEDs
  pinMode(buzzer, OUTPUT);


  Serial.println("Initializing the scale");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  
  scale.set_scale(-223.81);  // Adjust this based on your calibration
  scale.tare();  // Reset the scale to 0

  Serial.println("Scale ready");
}

void loop() {
  float weight = scale.get_units(15); // Get the average of 5 readings
  Serial.print("current Weight: ");
  Serial.print(weight);
  Serial.print(" g     ");

  if (weight >= gradeAweight) {
    // Good grade fruit, blink the good grade LED
    total_weight=total_weight+weight;
    gradeAcnt++;
    digitalWrite(buzzer,HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    scale.tare();
  } else {
    // Below grade fruit, blink the below-grade LED
    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
    scale.tare();
    
  }
  Serial.print("Total Weight: ");
  Serial.print(total_weight);
  Serial.print("     Grade A fruit counts: ");
  Serial.println(gradeAcnt);

  delay(500);  // Delay for 5 seconds before the next measurement
}

