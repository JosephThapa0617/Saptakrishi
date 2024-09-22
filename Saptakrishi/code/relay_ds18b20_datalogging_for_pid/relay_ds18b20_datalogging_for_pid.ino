#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO 15 on ESP32
#define ONE_WIRE_BUS 13
const int relayPin = 2;
unsigned long time;
String command="off";
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void) {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  sensors.begin();
  // Serial.println("DS18B20 Temperature Sensor Test");
}                                                                                                                                                                                                                                                                                                                                                                                                                                                         

void loop(void) {
  time=millis()/1000;

  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  // if (temperatureC == DEVICE_DISCONNECTED_C) {
  //   Serial.println("Error: Could not read temperature data");
  // } else {
  //   Serial.print(temperatureC);
  //   Serial.print(",");
  // }


  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');
    command.trim();  
    if (command == "off") {
      digitalWrite(relayPin, HIGH); 
    }
    else if (command == "on") {
      digitalWrite(relayPin, LOW);      
    }
    else {
      command="Null";
    }
  }  

String fin=String(time)+","+String(temperatureC)+","+command;
Serial.println(fin);
 delay(1000);
}