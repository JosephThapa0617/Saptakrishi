#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 9, 8);

byte arrow[8] = {
  B01000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B01000,
};

byte clear[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};



const int buttonPin3= 3;
const int buttonPin2 = 2;
// Variable to hold the count of button presses
volatile int colsPos = 0;
volatile int menuPos=0;
long debounce_time=300;
volatile unsigned long last_micros;
volatile bool interrupt=false;



void setup() {
  Serial.begin(9600);
  lcd.createChar(5, arrow);
  lcd.createChar(0, clear);
  lcd.begin(16, 2);  
  pinMode(buttonPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin2), buttonInterrupt2, FALLING);
  pinMode(buttonPin3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin3), buttonInterrupt3, FALLING);
   change_menu();
}

void loop() 
{
  Serial.println(menuPos);
  blink_arrow(0,colsPos,3); 
 
}



void change_menu()
{
  if (menuPos==0 || menuPos==1)
  { 
    lcd.clear();
    main_menu();
  }
  
  if (menuPos==2 || menuPos==3)
  { 
    lcd.clear();
    main_menu2();
  }
}



void blink_arrow(int col, int row, int count)
{
  int i=0;
   interrupt=false;
  while (i<count && !interrupt)
  {
  lcd.setCursor(col, row);
  lcd.write(byte(5));
  if (!interrupt)
  {delay(200);}
 
  lcd.setCursor(col, row);
  lcd.write(byte(0));
   if (!interrupt)
  {delay(200);}
  ++i;
  } 
}


void main_menu()
{ lcd.setCursor(1,0);
  lcd.print("Set Temp");
  lcd.setCursor(1,1);
  lcd.print("Current Temp");
}



void buttonInterrupt2() {
  // Increment the count variable
 if ((long)(micros()-last_micros)>=debounce_time*1000)
 {
  
  if (!menuPos==0)
  {menuPos=menuPos-1;}

  if ((menuPos % 2)==0)
  {colsPos=0;}

  else 
  {colsPos=1;}
  interrupt=true;
  change_menu();
  delay(500);
  last_micros=micros();

 } 
}



void buttonInterrupt3() {
  // Increment the count variable
 if ((long)(micros()-last_micros)>=debounce_time*1000)
 {

    menuPos=menuPos+1;

  if ((menuPos % 2)==0)
  {
    colsPos=0;
  }

  else 
  {
    colsPos=1;
  }

  change_menu();
    interrupt=true;
  last_micros=micros();
 } 
}



void main_menu2()
{ lcd.setCursor(1,0);
  lcd.print("Current Temp");
  lcd.setCursor(1,1);
  lcd.print("Settings");
}