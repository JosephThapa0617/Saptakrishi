#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "BluetoothSerial.h"

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

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

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


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

BluetoothSerial SerialBT;

int offsetTemp=0;
float temperatureC=0;
int setTemp=0;
unsigned long currentTime=0;
unsigned long prevTempRead=0;
unsigned long prevBluetoothRead=0;
unsigned long prevCompressorOn=0;
unsigned int bluetoothReadPeriod=200;
unsigned int tempReadPeriod = 5000;       //temperature is read every 5000 miliseconds
unsigned int compressorPeriod = 60000;
int pov=1;
String setPassword="ab";
int password_status=0;
bool compressor_on=false;


void getTemp();
void displayDigit(int digit);
void displaySign(bool isNegative);
void displaySignedNumber(int num);
void getBlueSet();
void compressor();


void setup(void) {
  Serial.begin(115200);
  sensors.begin();
  SerialBT.begin("ESP32testtt"); 
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

  pinMode(compressor_relay,OUTPUT);

  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);

  // Turn off both digits and sign display initially
  digitalWrite(first, LOW);
  digitalWrite(second, LOW);
  digitalWrite(signPin, LOW);

   digitalWrite(compressor_relay,HIGH);

  

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
}



void loop(void) {
  currentTime=millis();
  getTemp();
  displaySignedNumber(int(temperatureC));
  getBlueSet();
  compressor();

 
}

void compressor(){

  if ((currentTime-prevCompressorOn)>=compressorPeriod){
    prevCompressorOn=currentTime;
    Serial.println("Inside compressor");

    if(temperatureC>setTemp+offsetTemp){compressor_on=true;}
    else if(temperatureC<setTemp-offsetTemp){compressor_on=false;}
    digitalWrite(compressor_relay,!compressor_on);

  }
}



void getBlueSet(){
  if((currentTime-prevBluetoothRead)>=bluetoothReadPeriod){
  prevBluetoothRead=currentTime;
  if (SerialBT.available()) {
    String SetT=SerialBT.readString();
    setTemp=SetT.toInt();
  } 
   SerialBT.write(temperatureC);
}
}


void getTemp(){
  if ((currentTime-prevTempRead)>=tempReadPeriod)
  {
    prevTempRead=currentTime;
    sensors.requestTemperatures();
    temperatureC = sensors.getTempCByIndex(0);

    if (temperatureC == DEVICE_DISCONNECTED_C) {Serial.println("Error: Could not read temperature data");} 

    else {
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.print(" °C   ");
    Serial.println(setTemp);
  }
  } 
}

 

void displaySignedNumber(int num) {
  int absNumber = abs(num);   
  int tens = absNumber / 10; 
  int units = absNumber % 10; 

  displaySign(num < 0);

 
  digitalWrite(first, HIGH);  // Activate first digit
  displayDigit(tens);         // Show the tens digit
  delay(pov);                   // Short delay for persistence of vision
  digitalWrite(first, LOW);   // Deactivate first digit

  
  digitalWrite(second, HIGH);  // Activate second digit
  displayDigit(units);         // Show the units digit
  delay(pov);                    // Short delay for persistence of vision
  digitalWrite(second, LOW);   // Deactivate second digit
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