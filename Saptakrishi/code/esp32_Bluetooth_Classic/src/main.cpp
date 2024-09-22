 #include <Arduino.h>
#include "BluetoothSerial.h"
String setPassword="ab";
int password_status=0;
float curTemp=-11;
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); 

 while(password_status != 1) {
    Serial.println("The device started, now you can enter password");
      // setPassword=setPassword+"\r\n";
    while (SerialBT.connected()==false){}
    // SerialBT.println("Enter Password");
    while (SerialBT.available()==0){}
    String password=SerialBT.readString();

    Serial.println(password);


    if(password==setPassword){
      Serial.println("Correct Password");
      password_status=1;
      SerialBT.write(1);
    }

    else{
      Serial.println("Incorrect Password");
      // SerialBT.disconnect();
      SerialBT.write(0);
      password_status=0;
    }
}
}

void loop() {

if (SerialBT.available()) {
  String num=SerialBT.readString();
  Serial.println(num);
} 

SerialBT.write(curTemp);
 curTemp=curTemp+0.5;

delay(200);
}





