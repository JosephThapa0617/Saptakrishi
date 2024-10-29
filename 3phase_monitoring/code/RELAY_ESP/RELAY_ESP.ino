0void setup() {
  // put your setup code here, to run once:
  pinMode(32,OUTPUT);
  pinMode(33,OUTPUT);
  digitalWrite(32,HIGH);
  digitalWrite(33,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(32,LOW);
  delay(1000);
  digitalWrite(32,HIGH);
  delay(1000);
    digitalWrite(33,LOW);
  delay(1000);
  digitalWrite(33,HIGH);
  delay(1000);

}
