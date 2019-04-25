const int motorPin = 2;
void setup() {
  // put your setup code here, to run once:
pinMode (motorPin, OUTPUT);
}
void loop() {
   delay(1000);
  digitalWrite(motorPin, LOW);
     delay(1000);
  digitalWrite(motorPin, HIGH);
  delay(1000);
  digitalWrite(motorPin, LOW);
}

