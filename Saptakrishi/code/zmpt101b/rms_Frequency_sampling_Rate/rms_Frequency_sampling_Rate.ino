//768 241
int prevR=512;
int cnt=0;
unsigned long first;
unsigned long second;
float result;
float frequency;
float sum=0;
float squared;

int sampleSiz=400;
int i=0;

void setup()
{
  Serial.begin(115200);
  Serial.println("ins_vol:,rms:,freq:,samp_freq");
  

}

void loop()
{ 

  int val=analogRead(A0);
  unsigned long sampleX=micros();
   float vol=map(val,250,770,-292,292);
   Serial.print(vol);
   Serial.print(" ");
   

   if (i<sampleSiz)
   {
      squared=vol*vol;
      sum=sum+squared;
      ++i;
   }

   if (i==sampleSiz)
   {  
      result=sqrt(sum/i);
      i=0;
      sum=0;
   
   }

  Serial.print(result);
   Serial.print(" ");

 if(prevR<512 && val>512 && cnt==0)
 {
    first=micros();
    cnt=1;
 }

 if (prevR>512 && val<512 && cnt==1)
 {
    second=micros();
    frequency=float(500000/(second-first));
    cnt=0;

 }

 Serial.print(frequency);
 Serial.print(" ");

  prevR=val;


   Serial.println(1000000/(micros()-sampleX));
}
