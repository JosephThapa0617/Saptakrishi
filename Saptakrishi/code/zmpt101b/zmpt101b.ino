
int prevR=512;

void setup()
{
  Serial.begin(115200);

}

void loop()
{ unsigned long prev=micros();
   Serial.print(analogRead(A0));
   Serial.print(" ");
   Serial.println(micros()-prev );

}
