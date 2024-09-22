//for esp32
#include <LiquidCrystal.h>

LiquidCrystal lcd(13, 12, 14, 27, 26, 25);//[ RS, EN, D4, D5, D6, D7 ]


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);  
}

void loop() 
{
   lcd.print(" Hello world!");

	
	lcd.setCursor(0, 1);
	lcd.print(" Testing");
 
 
}


