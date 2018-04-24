/*
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>
#include <DS3231.h>
#include "HX711.h"

#define DOUT  3
#define CLK  2

DS3231  rtc(SDA, SCL);
HX711 scale(DOUT, CLK);

float calibration_factor = -36000; //-7050 worked for my 440lb max scale setup

const int chipSelect = 4;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0

  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  rtc.begin();

  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(12, 0, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(1, 1, 2014);   // Set the date to January 1st, 2014

  long zero_factor = scale.read_average(); //Get a baseline reading
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  */

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

void loop() {
  // make a string for assembling the data to log:
  String dataString = "";


  // read three sensors and append to the string:
    //scale.set_scale(calibration_factor); //Adjust to this calibration factor
    //int scaleUnit = scale.get_units();
    //dataString = scaleUnit;

    
    
    
    
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    
    dataFile.println(scale.get_units());
    dataFile.println(rtc.getDateStr());
    dataFile.println(rtc.getTimeStr());
    dataFile.close();
    
    // print to the serial port too:
    Serial.println(scale.get_units());
    // Send date
    Serial.println(rtc.getDateStr());
    //Send time
    Serial.println(rtc.getTimeStr());
    
    delay(250);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}
