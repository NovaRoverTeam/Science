/***************************************************************************
  Adafruit AS7262 6-Channel Visible Light Sensor for photometer
  I2C address 0x49
  Sensor will read all wavelengths at 1 second intervals, as well as sensor temperature
  WIll then write data to file 'read.txt'
  Please start serial plotter to view graphed data
  Adapted from "AS7262_test" Written by Dean Miller for Adafruit Industries.
  Must start with LED switched off!
 ***************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "Adafruit_AS726x.h"
int LEDpin = 3;
File myFile;
Adafruit_AS726x ams;
//buffer to hold raw values
uint16_t sensorValues[AS726x_NUM_CHANNELS];
//buffer to hold calibrated values
float calibratedValues[AS726x_NUM_CHANNELS];

void setup() {
  Serial.begin(9600);
  while (!Serial);
  pinMode(LEDpin, OUTPUT);
  //begin and make sure we can talk to the sensor
  if (!ams.begin()) {
    Serial.println("could not connect to sensor! Please check your wiring.");
    while (1);
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(8)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("read.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to read.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening read.txt");
  }
}
void loop() {
  ams.startMeasurement(); //begin a measurement
  bool rdy = false;  //wait till data is available
  while (!rdy) {
    delay(5);
    rdy = ams.dataReady();
  }
  digitalWrite(LEDpin, HIGH);
  delay(1000);
  myFile = SD.open("read.txt", FILE_WRITE);
  if (myFile) {
    //read the values!
    ams.readRawValues(sensorValues);
    ams.readCalibratedValues(calibratedValues);

   myFile.print(sensorValues[AS726x_VIOLET]);  myFile.print(" Red_cal: ");myFile.print(calibratedValues[AS726x_VIOLET]);
   myFile.print(sensorValues[AS726x_BLUE]); myFile.print(" Blue_cal: "); myFile.print(calibratedValues[AS726x_BLUE]);
   myFile.print(sensorValues[AS726x_GREEN]); myFile.print(" Violet_cal: "); myFile.print(calibratedValues[AS726x_GREEN]);
   myFile.print(sensorValues[AS726x_YELLOW]); myFile.print(" Green_cal: "); myFile.print(calibratedValues[AS726x_YELLOW]);
   myFile.print(sensorValues[AS726x_ORANGE]); myFile.print(" Orange_cal: "); myFile.print(calibratedValues[AS726x_ORANGE]);
   myFile.print(sensorValues[AS726x_RED]); myFile.print(" Red_cal: "); myFile.print(calibratedValues[AS726x_RED]);
    myFile.println();
    myFile.close();
    delay(1000);
  } else {
    // if the file didn't open, print an error:
    Serial.println("Error opening file in loop.");
  }
}
