
int prevR=512;
int cnt=0;
unsigned long prevT;
unsigned long curT;

void setup()
{
  Serial.begin(115200);

}

void loop()
{ 
  
  int val=analogRead(A0);
  // Serial.print(val);

 if(prevR<512 && val>512)
 {
    curT=millis();
    float x=1000/(curT-prevT);

    Serial.println(x);
 }
  prevT=curT;
  prevR=val;
 
}
