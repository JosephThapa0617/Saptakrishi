#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "BluetoothSerial.h"

// Pin definitions
#define A 15
#define B 2
#define C 4
#define D 16
#define E 17
#define F 5
#define G 18

#define first   21  // Control pin for the first digit
#define second  22 // Control pin for the second digit
#define signPin 19 // Control pin for the sign display

#define ONE_WIRE_BUS 23
#define compressor_relay 33
#define voltagePin 34 // Pin connected to ZMPT101B output

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


// Global variables
float temperatureC = 0;
float rmsVoltage = 0;
int setTemp = 0;
bool compressor_on = false;
unsigned long currentTime = 0;
unsigned long prevTempRead = 0;
unsigned long prevBluetoothRead = 0;
unsigned long prevCompressorOn = 0;
unsigned int bluetoothReadPeriod = 200;
unsigned int tempReadPeriod = 5000;
unsigned int compressorPeriod = 60000;
int pov=1;
String setPassword="ab";
int password_status=0;



OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
BluetoothSerial SerialBT;

// Digit and sign data
byte digits[10][7] = {
  {LOW, LOW, LOW, LOW, LOW, LOW, HIGH},  // 0
  {HIGH, LOW, LOW, HIGH, HIGH, HIGH, HIGH},  // 1
  {LOW, LOW, HIGH, LOW, LOW, HIGH, LOW},  // 2
  {LOW, LOW, LOW, LOW, HIGH, HIGH, LOW},  // 3
  {HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW},  // 4
  {LOW, HIGH, LOW, LOW, HIGH, LOW, LOW},  // 5
  {LOW, HIGH, LOW, LOW, LOW, LOW, LOW},  // 6
  {LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH},  // 7
  {LOW, LOW, LOW, LOW, LOW, LOW, LOW},  // 8
  {LOW, LOW, LOW, LOW, HIGH, LOW, LOW}   // 9
};

byte sign[2][7] = {
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH},  // Blank (for positive numbers)
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW}    // '-' sign (active low on G segment)
};

// Function prototypes
void getTemp();
void displayDigit(int digit);
void displaySign(bool isNegative);
void displaySignedNumber(int num);
void getBlueSet();
void compressor();
void readVoltage(void *pvParameters);
void coreTask(void *pvParameters);

// Create the main task to run on Core 0
void setup() {
  Serial.begin(115200);
  
  // Initialize temperature and Bluetooth
  sensors.begin();
  SerialBT.begin("ESP32testtt");

  // Pin setup for seven-segment display and compressor control
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(first, OUTPUT);
  pinMode(second, OUTPUT);
  pinMode(signPin, OUTPUT);
  pinMode(compressor_relay, OUTPUT);
  digitalWrite(compressor_relay, HIGH); // Turn off compressor

  // Pin setup for voltage sensing
  pinMode(voltagePin, INPUT);

  while(password_status != 1) {
    Serial.println("The device started, now you can enter password");
    while (SerialBT.connected()==false){}

    while (SerialBT.available()==0){}
    String password=SerialBT.readString();

    Serial.println(password);


    if(password==setPassword){
      Serial.println("Correct Password");
      password_status=1;
      SerialBT.write(1);
      digitalWrite(compressor_relay,LOW);
    }

    else{
      Serial.println("Incorrect Password");
      SerialBT.write(0);
      password_status=0;
    }
}

  // Create the main task on Core 0
  xTaskCreatePinnedToCore(
    coreTask,   // Task function
    "Core Task",// Task name
    10000,      // Stack size
    NULL,       // Parameters
    1,          // Priority
    NULL,       // Task handle
    0           // Core 0
  );

  // Create a task for voltage sensing on Core 1
  xTaskCreatePinnedToCore(
    readVoltage,   // Task function
    "Voltage Task",// Task name
    10000,         // Stack size
    NULL,          // Parameters
    1,             // Priority
    NULL,          // Task handle
    1              // Core 1
  );
}

// Main task that replaces loop() on Core 0
void coreTask(void *pvParameters) {
  while (true) {
    currentTime = millis();
    getTemp();
    displaySignedNumber(int(temperatureC));
    getBlueSet();
    compressor();
    vTaskDelay(10 / portTICK_PERIOD_MS);  // Small delay for task scheduling
  }
}

// Voltage sensing task on Core 1
void readVoltage(void *pvParameters) {
  while (1) {
    // int sensorValue = analogRead(voltagePin);
    // rmsVoltage = (sensorValue * 3.3) / 4095;  // Convert ADC value to voltage (assuming 3.3V reference)
    // Serial.print("RMS Voltage: ");
    // Serial.println(rmsVoltage);
    // vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay 1 second
  }
}

void loop(){}
// Compressor control logic
void compressor() {
  if ((currentTime - prevCompressorOn) >= compressorPeriod) {
    prevCompressorOn = currentTime;
    if (temperatureC > setTemp) { compressor_on = true; }
    else if (temperatureC < setTemp) { compressor_on = false; }
    digitalWrite(compressor_relay, !compressor_on);
  }
}

// Bluetooth communication for set temperature
void getBlueSet() {
  if ((currentTime - prevBluetoothRead) >= bluetoothReadPeriod) {
    prevBluetoothRead = currentTime;
    if (SerialBT.available()) {
      String SetT = SerialBT.readString();
      setTemp = SetT.toInt();
    }
    SerialBT.write(temperatureC);
  }
}

// Temperature reading function
void getTemp() {
  if ((currentTime - prevTempRead) >= tempReadPeriod) {
    prevTempRead = currentTime;
    sensors.requestTemperatures();
    temperatureC = sensors.getTempCByIndex(0);
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.print(" °C   ");
    Serial.println(setTemp);
  }
}

// Seven-segment display functions
void displaySignedNumber(int num) {
  int absNumber = abs(num);
  int tens = absNumber / 10;
  int units = absNumber % 10;
  displaySign(num < 0);
  digitalWrite(first, HIGH); displayDigit(tens); delay(pov); digitalWrite(first, LOW);
  digitalWrite(second, HIGH); displayDigit(units); delay(pov); digitalWrite(second, LOW);
}

void displayDigit(int digit) {
  digitalWrite(A, digits[digit][0]);
  digitalWrite(B, digits[digit][1]);
  digitalWrite(C, digits[digit][2]);
  digitalWrite(D, digits[digit][3]);
  digitalWrite(E, digits[digit][4]);
  digitalWrite(F, digits[digit][5]);
  digitalWrite(G, digits[digit][6]);
}

void displaySign(bool isNegative) {
  digitalWrite(signPin, HIGH);
  if (isNegative) {
    digitalWrite(A, sign[1][0]);
    digitalWrite(B, sign[1][1]);
    digitalWrite(C, sign[1][2]);
    digitalWrite(D, sign[1][3]);
    digitalWrite(E, sign[1][4]);
    digitalWrite(F, sign[1][5]);
    digitalWrite(G, sign[1][6]);
  } else {
    digitalWrite(A, sign[0][0]);
    digitalWrite(B, sign[0][1]);
    digitalWrite(C, sign[0][2]);
    digitalWrite(D, sign[0][3]);
    digitalWrite(E, sign[0][4]);
    digitalWrite(F, sign[0][5]);
    digitalWrite(G, sign[0][6]);
  }
  delay(pov);
  digitalWrite(signPin, LOW);
}
