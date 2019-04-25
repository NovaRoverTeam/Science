#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "Adafruit_TSL2561_U.h"
#include "Adafruit_VEML6075.h"
#include "SparkFunBME280.h"
#include "Adafruit_CCS811.h"
#include "SoftwareSerial.h"
int analoguePin [] = {A1};
int val = 0;
int MethaneRaw;
int Methaneppb;
BME280 mySensor;
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
Adafruit_VEML6075 uv = Adafruit_VEML6075();
Adafruit_CCS811 ccs;
const int chipSelect = 8;
File myFile;

//Configuring TSL light sensor
void configureSensor(void)
{ tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
}

void setup(void)
{
  Serial.begin(9600);
  //pin for methane sensor readings
  pinMode (analoguePin [A1], INPUT);
  while (!Serial) {
    ;
  }
//Initialise SD card and test
  Serial.print("Initializing SD card...");
  if (!SD.begin(8)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("data.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to data.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening data.txt");
  }
  // TSL2561 start up code  //use tsl.begin() to default to Wire, //tsl.begin(&Wire2) directs api to use Wire2, etc.
  if (!tsl.begin())
  {
    Serial.print("No TSL2561 detected, check wiring or I2C ADDR!");
    //while (1);
  }

  //BMS280 start up code:
  Wire.begin();
  if (mySensor.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("No BME280 detected, check wiring or I2C ADDR!");
    //while (1);
  }
  // VEML6075 start up code
  configureSensor();
  if (! uv.begin()) {
    Serial.println("No VEML6075 detected, check wiring or I2C ADDR!");
  }
 // uv.setHighDynamic(false);
  uv.setForcedMode(false);
  // Set the calibration coefficients
  uv.setCoefficients(2.22, 1.33,  // UVA_A and UVA_B coefficients
                     2.95, 1.74,  // UVB_C and UVB_D coefficients
                     0.001461, 0.002591); // UVA and UVB responses 
}

void loop(void) {
  //Openinf file on SD card
  myFile = SD.open("data.txt", FILE_WRITE);
// Starting new sensor  events
  sensors_event_t event;
  tsl.getEvent(&event);
  MethaneRaw = analogRead(A1);
  Methaneppb = Methane_ppb(MethaneRaw);
  if (myFile) {
    // if file available, write data to Serial
    if (event.light)
    { Serial.print("light: "); Serial.print(event.light); Serial.print("lux, ");
    }
    else
    {
      Serial.print(" Lux Sensor overload ");
    }
    Serial.print("Hum: ");  Serial.print(mySensor.readFloatHumidity(), 0); Serial.print("%, ");
    Serial.print(" Pres: ");  Serial.print(mySensor.readFloatPressure(), 0); Serial.print(" Pa, ");
    //Serial.print(" Alt: ");  Serial.print(mySensor.readFloatAltitudeMeters(), 1); Serial.print(" m, ");
    Serial.print(" Temp: ");  Serial.print(mySensor.readTempC(), 2); Serial.print("°C, ");
    Serial.print("RawUVA: "); Serial.print(uv.readUVA()); Serial.print(", ");
    Serial.print("RawUVB: "); Serial.print(uv.readUVB()); Serial.print(", ");
    Serial.print("UVI: "); Serial.print(uv.readUVI()); Serial.print(", ");
    //Serial.print("Meth_Raw: "); Serial.print(MethaneRaw); Serial.print(" ppm, ");
    Serial.print("Meth_ppb: "); Serial.print(Methaneppb); Serial.print(" ppb, ");
    Serial.print("CO2: ");  Serial.print(ccs.geteCO2()); Serial.print(" ppm, ");
    //Serial.print("TVOC: ");  Serial.print(ccs.getTVOC()); Serial.print(" ppb, ");
    Serial.println(); 
    // write data to SD card
    if (event.light)
    { myFile.print("light: "); myFile.print(event.light); myFile.print("lux, ");
    }
    else
    {
      myFile.print("Lux Sensor overload");
    }
    myFile.print("Hum: ");  myFile.print(mySensor.readFloatHumidity(), 0); myFile.print("%, ");
    myFile.print(" Pres: ");  myFile.print(mySensor.readFloatPressure(), 0); myFile.print(" Pa, ");
    //myFile.print(" Alt: ");  myFile.print(mySensor.readFloatAltitudeMeters(), 1); myFile.print(" m, ");
    myFile.print(" Temp: ");  myFile.print(mySensor.readTempC(), 2); myFile.print("°C, ");
    myFile.print("RawUVA: "); myFile.print(uv.readUVA()); myFile.print(", ");
    myFile.print("RawUVB: "); myFile.print(uv.readUVB()); myFile.print(", ");
    myFile.print("UVI: "); myFile.print(uv.readUVI()); myFile.print(", ");
   // myFile.print("Meth_Raw: "); myFile.println(MethaneRaw); myFile.print(" ppb, ");
    myFile.print("Meth_ppb: "); myFile.println(Methaneppb); myFile.print(" ppb, ");
    myFile.print("CO2: ");  myFile.print(ccs.geteCO2()); myFile.print(" ppm, ");
    //myFile.print("TVOC: ");  myFile.print(ccs.getTVOC()); myFile.print(" ppb, ");
    myFile.println();
    // close the file
    myFile.close();
  }
  else {
    Serial.println("Error opening file in loop.");
  }
  delay(5000);
}
int Methane_ppb(double Rawval) {
  double ppb = (10.938 * exp(1.7742 * (Rawval * 5 / 4095)))*1000;
  return ppb;
}
