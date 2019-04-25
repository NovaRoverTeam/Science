int motorPin1 = 8;
int motorPin2 = 9;
int motorPin3 = 10;
int motorPin4 = 11;
int delayTime = 5;
#include <Wire.h>
#include "Adafruit_AS726x.h"

//create the object
Adafruit_AS726x ams;

//buffer to hold raw values
uint16_t sensorValues[AS726x_NUM_CHANNELS];

//buffer to hold calibrated values (not used by default in this example)
//float calibratedValues[AS726x_NUM_CHANNELS];

int LEDpin = 3;
 
void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(LEDpin, OUTPUT);
   Serial.begin(9600);
  while(!Serial);
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  //begin and make sure we can talk to the sensor
  if(!ams.begin()){
    Serial.println("could not connect to sensor! Please check your wiring.");
    while(1);
  }
}
 
void loop() {
     digitalWrite(LEDpin, HIGH);
  digitalWrite(motorPin4, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, LOW);
  delay(delayTime);
  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, LOW);
  delay(delayTime);
  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin1, LOW);
  delay(delayTime);
      digitalWrite(LEDpin, LOW);
  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, HIGH);
  delay(delayTime);
  
  //read the device temperature
//  uint8_t temp = ams.readTemperature();
  
  ams.startMeasurement(); //begin a measurement
  
  //wait till data is available
  bool rdy = false;
  while(!rdy){
    delay(5);
    rdy = ams.dataReady();
  }

  //read the values!
  ams.readRawValues(sensorValues);
  //ams.readCalibratedValues(calibratedValues);
 // Serial.print("Temp: "); Serial.print(temp);
  Serial.print(" Violet: "); Serial.print(sensorValues[AS726x_VIOLET]);
  Serial.print(" Blue: "); Serial.print(sensorValues[AS726x_BLUE]);
  Serial.print(" Green: "); Serial.print(sensorValues[AS726x_GREEN]);
  //Serial.print(" Yellow: "); Serial.print(sensorValues[AS726x_YELLOW]);
  //Serial.print(" Orange: "); Serial.print(sensorValues[AS726x_ORANGE]);
  //Serial.print(" Red: "); Serial.print(sensorValues[AS726x_RED]);
  Serial.println();
  Serial.println();
}
