const int analogPin = 34;  // Pin where ZMPT101B output (through voltage divider) is connected

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);  // Set the resolution to 12 bits (ESP32)
}

void loop() {
  int sensorValue = analogRead(analogPin);  // Read the analog input
  float voltage = (sensorValue / 4095.0) * 5.0;  // Adjust calculation for 5V range (after scaling)
  
  // Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  // Serial.print(" - Voltage: ");
  //  Serial.println(voltage, 3);  // Print voltage in 3 decimal places
  
  delay(1);
}
