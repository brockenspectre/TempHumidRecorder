#include <SoftwareSerial.h>
#include <SparkFunSerialGraphicLCD.h>
#include <SHT1X.h>

/******************************************************************************
SHT15 Example
Joel Bartlett @ SparkFun Electronics
16 Sept 2015
https://github.com/sparkfun/SparkFun_ISL29125_Breakout_Arduino_Library

This example shows how to get the temperature in F or C and humidity
Developed/Tested with:
SparkFun RedBoard 
Arduino IDE 1.6.5

Connections:
GND  -> A2
Vcc  -> A3
DATA -> A4
SCK  -> A5

Requires:
SparkFun_SHT1X Arduino Library
https://github.com/sparkfun/SHT15_Breakout/

This code is beerware.
Distributed as-is; no warranty is given. 
******************************************************************************/
#include <SPI.h>
#include <SparkFunDS3234RTC.h>

// Comment out the line below if you want month printed before date.
// E.g. October 31, 2016: 10/31/16 vs. 31/10/16
#define PRINT_USA_DATE

//////////////////////////////////
// Configurable Pin Definitions //
//////////////////////////////////
#define DS13074_CS_PIN 10 // DeadOn RTC Chip-select pin

//variables for storing values
float tempC = 0;
float tempF = 0;
float humidity = 0;

//Create an instance of the SHT1X sensor
SHT1x sht15(A4, A5);//Data, SCK

//delacre output pins for powering the sensor
int power = A3;
int gnd = A2;

SoftwareSerial portOne(8,9);
LCD LCD;

void setup()
{
  Serial.begin(9600); // Open serial connection to report values to host

  delay(1000); //Wait a second for OpenLog to init

  pinMode(power, OUTPUT);
  pinMode(gnd, OUTPUT);

  digitalWrite(power, HIGH);
  digitalWrite(gnd, LOW);

  portOne.begin(115200);
  LCD.setBacklight(0);
  LCD.toggleSplash();
  delay(3000);
  LCD.clearScreen();

 // Call rtc.begin([cs]) to initialize the library
 // The chip-select pin should be sent as the only parameter
  rtc.begin(DS13074_CS_PIN);

  Serial.println(" Time Date Temp(F)   Humidity(%)");
}
//-------------------------------------------------------------------------------------------
void loop()
{
    // Call rtc.update() to update all rtc.seconds(), rtc.minutes(),
  // etc. return functions.
  rtc.update();

  readSensor();
  printOut();
  delay(60000);
}
//-------------------------------------------------------------------------------------------
void readSensor()
{
  // Read values from the sensor
  tempC = sht15.readTemperatureC();
  tempF = sht15.readTemperatureF();
  humidity = sht15.readHumidity();  
}
//-------------------------------------------------------------------------------------------
void printOut()
{
  Serial.print(String(rtc.hour()) + ":"); // Print hour
  if (rtc.minute() < 10)
    Serial.print('0'); // Print leading '0' for minute
  Serial.print(String(rtc.minute()) + ":"); // Print minute
  if (rtc.second() < 10)
    Serial.print('0'); // Print leading '0' for second
  Serial.print(String(rtc.second())); // Print second
  Serial.print(" ");

  Serial.print(String(rtc.month()) + "/" +   // Print month
                 String(rtc.date()) + "/");  // Print date

  Serial.print(String(rtc.year()));        // Print year
  Serial.print(" ");
  
  Serial.print(tempF);
  Serial.print(" ");
  Serial.println(humidity); 
  LCD.setHome();
  LCD.clearScreen();
  portOne.print(tempF);
  portOne.print("F  ");
  portOne.print(humidity);
  portOne.print("%");
}
