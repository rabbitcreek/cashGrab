#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "RTClib.h"
#define DONEPIN  12
#define inputPin 11
#define VBATPIN A9
//#include "SSD1306Ascii.h" // https://github.com/greiman/SSD1306Ascii
#include "SSD1306AsciiWire.h" // https://github.com/greiman/SSD1306Ascii
#define cardSelect 4
float temperatureF1 = 0;
float temperatureF2 = 0;
RTC_DS3231 rtc;
#define I2C_ADDRESS 0x3C
 // create oled dispaly object
const int oneWireBus = 6; 
OneWire oneWire(oneWireBus); 
File logfile;
char filename[15];
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
void setup(){
  // Start the Serial Monitor
  Wire.begin();
  Serial.begin(115200);
  pinMode(DONEPIN, OUTPUT);
  pinMode(inputPin, INPUT);
  digitalWrite(inputPin, LOW);
  digitalWrite(DONEPIN, LOW);
  // Start the DS18B20 sensor
  sensors.begin();
 /*
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
 
  oled.set400kHz();  
  oled.setFont(Adafruit5x7); 
  oled.setScroll(true);   
  oled.clear();
  oled.home();
  oled.set1X();
 */
  
if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
   
  }
  strcpy(filename, "ANALOG02.TXT");//this section sets up new files for each new reading
  /*
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }
  */
   
  if( ! logfile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
    
  }
  Serial.print("Writing to "); 
  Serial.println(filename);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
 //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
   
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

}
}
void loop(){
  logfile = SD.open(filename, FILE_WRITE);
   DateTime now = rtc.now();
   sensors.requestTemperatures(); 
   delay(100);
  //float temperatureC = sensors.getTempCByIndex(0);
   temperatureF1 = sensors.getTempFByIndex(0);
   
  delay(100);
   temperatureF2 = sensors.getTempFByIndex(1);
  delay(100);
  Serial.print("T1:  ");
  Serial.println(temperatureF1);
  Serial.print("T2:  ");
  Serial.println(temperatureF2);
  float fileSize = logfile.size();
 

   
float measuredvbat = analogRead(VBATPIN);
measuredvbat *= 2;    // we divided by 2, so multiply back
measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
measuredvbat /= 1024; // convert to voltage
Serial.print("VBat: " ); Serial.println(measuredvbat);
 
  // log time
  //logfile.print(now.get()); // seconds since 2000
  logfile.print("T1: ");
  logfile.print(temperatureF1);
  logfile.print("  T2: ");
  logfile.print(temperatureF2);
  logfile.print("  ");
  logfile.print(now.year(), DEC);
  logfile.print("/");
  logfile.print(now.month(), DEC);
  logfile.print("/");
  logfile.print(now.day(), DEC);
  logfile.print(" ");
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  logfile.print(now.second(), DEC);
  logfile.print(" VBAT: ");
  logfile.println(measuredvbat);
  
  logfile.flush();
  logfile.close();

  if(digitalRead(inputPin)){
  SSD1306AsciiWire oled;
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.set400kHz();  
  oled.setFont(Adafruit5x7); 
  oled.setScroll(true);   
  oled.clear();
  oled.home();
  oled.set1X();
  oled.print("T1 ");
  oled.print(temperatureF1);
  oled.print(" T2 ");
  oled.println(temperatureF2);
  oled.print(now.year(), DEC);
  oled.print("/");
  oled.print(now.month(), DEC);
  oled.print("/");
  oled.println(now.day(), DEC);
  oled.print(" ");
  oled.print(now.hour(), DEC);
  oled.print(":");
  oled.print(now.minute(), DEC);
  oled.print(":");
  oled.println(now.second(), DEC);
  oled.print("File Size  ");
  oled.println(fileSize);
  oled.print("VBat:  ");
  oled.println(measuredvbat);
  delay(5000);
  oled.clear();
  }
  

 
  Serial.print(temperatureF1);
  Serial.println("ºC");
  Serial.print(temperatureF2);
  Serial.println("ºF");
  delay(500);
  //logfile.print("CO2 PPM  "); logfile.print(mySensor.ppm);
      //logfile.print("  O2%");
      //logfile.print("  ");
  if(!digitalRead(inputPin)){   
       while (1) {
    digitalWrite(DONEPIN, HIGH);
    delay(1);
    digitalWrite(DONEPIN, LOW);
    delay(1);
  }
  }
  
    
     
}

  
  


