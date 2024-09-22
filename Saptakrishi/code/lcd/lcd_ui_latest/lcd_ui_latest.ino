#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 15
#define CLK 18
#define DT 19
#define SW 21
char page = 'h';   //home page:h, menu page:m and sub menu page:s
int lastCLK = HIGH;
int currentCLK;
int menuOption=0;
float currentTemperature=-24;
float setTemperature=10;
unsigned long lastButtonPress = 0; 
const unsigned long debounceDelay = 50;


LiquidCrystal lcd(13, 12, 14, 27, 26, 25);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);



void setup()
{
  Serial.begin(115200);
  sensors.begin();
  lcd.begin(16,2);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  displayHomePage();
}



void loop()
{
  int btnState = digitalRead(SW);
  if(btnState==LOW)
  {

    if (millis() - lastButtonPress>debounceDelay){
        Serial.println("button pressed  ");
        // Serial.println(page);

        if(page == 'h'){
        displayMenu();
        page='m';
        }

        else if (page == 'm'){
        // Serial.println("IN MENU PAGE");
          if (menuOption == 2){
            displayHomePage();
            page='h';  
          }

          if (menuOption == 0){
            displaySetTemp();
            page='s';
          }
        }


        else if (page == 's'){
          displayHomePage();
          page='h';
        }

    }
    lastButtonPress=millis();
  }





  if(page!='h'){

      currentCLK=digitalRead(CLK);
      if (currentCLK != lastCLK){
        if(digitalRead(DT) != currentCLK){
            if(page == 'm'){
              if (menuOption<2)
                menuOption++;
                Serial.println(menuOption);
                changeMenu();
            }

            else if (page == 's' && menuOption==0){
              if (menuOption<25)
                setTemperature++;
                lcd.setCursor(0,1);
                lcd.write("                ");
                lcd.setCursor(0,1);
                lcd.print(">");
                lcd.print(setTemperature,1);
                lcd.print((char)223); 
                lcd.print("C");     
            }
        }

        else{
          if(page == 'm'){
            if(menuOption>0)
              menuOption--;
              Serial.println(menuOption);
              changeMenu();
          }
          else if (page == 's' && menuOption==0){
              if (menuOption>-10)
                setTemperature--;
                lcd.setCursor(0,1);
                lcd.write("                ");
                lcd.setCursor(0,1);
                lcd.print(">");
                lcd.print(setTemperature,1);
                lcd.print((char)223); 
                lcd.print("C");
            }
        }
        lastCLK=currentCLK;
      }
  } 

  if (page == 'h')
  {
      sensors.requestTemperatures();
      currentTemperature = sensors.getTempCByIndex(0);
    if (currentTemperature == DEVICE_DISCONNECTED_C) {
      Serial.println("Error: Could not read temperature data");
    } 
    else {
      Serial.print("Temperature: ");
      Serial.print(currentTemperature);
      Serial.println(" °C");
      
    }
    displayHomePage();
    delay(50);
  
  } 

  
}






void displayHomePage() 
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CurTemp  Target");
  lcd.setCursor(0, 1);
  lcd.print(currentTemperature, 1); 
  lcd.print((char)223); 
  lcd.print("C");

  lcd.setCursor(9, 1);
  lcd.print(setTemperature, 1); 
  lcd.print((char)223); 
  lcd.print("C");
}



void displayMenu(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select Option:");
  lcd.setCursor(0, 1);
  lcd.print("> Set Temp");
}


void changeMenu(){
  lcd.clear();
  switch (menuOption){
  case 0:
    lcd.print("Select Option:");
    lcd.setCursor(0, 1);
    lcd.print(">Set Temp");
    break;
  case 1:
    lcd.print("Select Option:");
    lcd.setCursor(0, 1);
    lcd.print(">Alerts");
    break;
  case 2:
    lcd.print("Select Option:");
    lcd.setCursor(0, 1);
    lcd.print(">Exit");
    break;
  }
}

void displaySetTemp(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set Temp:");
  lcd.setCursor(0,1);
  lcd.print(">");
  lcd.print(setTemperature,1);
  lcd.print((char)223); 
  lcd.print("C");

}
