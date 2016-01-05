//Some libraries
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <SimpleTimer.h>

//Pin Config
SoftwareSerial esp(2, 3);
int pinRelay = 6;
int pinMoisture = 4;
int LED_SD = 5;
const int chipSelect = 10;

//RTC
RTC_DS1307 RTC;

//Variable
unsigned long pulse;
float tetha;
SimpleTimer timer;
long period = 900000;
unsigned long average = 0;          // the pulse

void setup()
{
	Serial.begin(115200);
  if(!Serial)
    Serial.end();
	Wire.begin();
	RTC.begin();
  
	pinMode(pinRelay, OUTPUT);
	pinMode(LED_SD, OUTPUT);

	// if the file is available and one hour passed, write to it:
	timer.setInterval(period, logger);
	timer.setInterval(1000, printmoist);

	if (! RTC.isrunning()) {
	    Serial.println("RTC is NOT running!");
	    // following line sets the RTC to the date & time this sketch was compiled
      RTC.adjust(DateTime(__DATE__, __TIME__));
  	}
  	Serial.print("Initializing SD card...");
	
	  // see if the card is present and can be initialized:
	  if (!SD.begin(chipSelect)) {
	      Serial.println("Card failed, or not present");
		    // don't do anything more:
		    return;
	  }
	  Serial.println("card initialized.");

	  delay(1000);

	  File dataFile = SD.open("DATALOG.TXT", FILE_WRITE);
    DateTime now = RTC.now();
	  if (dataFile) {
  		digitalWrite(LED_SD, LOW);
 		  Serial.print("log ");
	  	Serial.print(now.month(), DEC);
		  Serial.print('/');
		  Serial.print(now.day(), DEC);
		  Serial.print(' ');
		  Serial.print(now.hour(), DEC);
		  Serial.print(':');
		  Serial.print(now.minute(), DEC);
		  Serial.println("");
		  }  else {
		  Serial.print("file not exist");
		  digitalWrite(LED_SD, HIGH);
	  }
	  dataFile.close();
}

void printmoist() {
	Serial.println(tetha);
}

void logger(){
    DateTime now = RTC.now();
    File dataFile = SD.open("DATALOG.TXT", FILE_WRITE);
    if(dataFile){
		digitalWrite(LED_SD, LOW);
		//Date Time
		dataFile.print(now.year(), DEC);
		dataFile.print('/');
		dataFile.print(now.month(), DEC);
		dataFile.print('/');
		dataFile.print(now.day(), DEC);
    dataFile.print(' ');
    dataFile.print(now.hour(), DEC);
    dataFile.print(':');
    dataFile.print(now.minute(), DEC);
    dataFile.print(':');
    dataFile.print(now.second(), DEC);
    dataFile.print('\t');

    //Moisture
	  dataFile.println(String(tetha,4));
    } else {
      Serial.println("file not exist");
	    digitalWrite(LED_SD, HIGH);
    }
	
  	dataFile.close();
}

void loop()
{
  timer.run();
  
  //Read pulse
	average = pulseIn(pinMoisture, HIGH);
	tetha = 0.0000002*(average*average) - (0.0007*average) + 0.6012;
  tetha = constrain(tetha, 0.0f, 0.5f);
	delay(1);
}
