#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO 15 on ESP32
#define ONE_WIRE_BUS 23

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

void setup(void) {
  // Start serial communication
  Serial.begin(115200);
  
  // Start the DS18B20 sensor
  sensors.begin();
  
  // Print a start message
  Serial.println("DS18B20 Temperature Sensor Test");
}

void loop(void) {
  // Request temperature measurement from the sensor
  sensors.requestTemperatures();
  
  // Fetch the temperature in Celsius
  float temperatureC = sensors.getTempCByIndex(0);
  
  // Check if the temperature reading is valid
  if (temperatureC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data");
  } else {
    // Print the temperature to the Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C");
  }
  
  // Wait 1 second before the next measurement
  delay(1000);
}