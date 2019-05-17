#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#define I2C_ADDR 0x27 
#define BACKLIGHT_PIN 3
LiquidCrystal_I2C lcd(I2C_ADDR,2,1,0,4,5,6,7);
SoftwareSerial serial_connection(10, 11); //RX=pin 10, TX=pin 11
TinyGPSPlus gps;

void setup()
{
Serial.begin(9600);
serial_connection.begin(9600);
Serial.println("GPS Start");


lcd.begin (20,4); 
lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
lcd.setBacklight(HIGH);
lcd.home (); 
lcd.setCursor(4,0);
lcd.print("GPS TEST V!1"); 
lcd.setCursor(1,1);
lcd.print("Lat :"); 
lcd.setCursor(1,2);
lcd.print("Lon :"); 
}


void loop()
{
while(serial_connection.available())
{
gps.encode(serial_connection.read());
}
if(gps.location.isUpdated())
{
  
lcd.setCursor(6,1);
lcd.print(gps.location.lat(), 6); 
lcd.setCursor(6,2);
lcd.print(gps.location.lng(), 6); 

}

delay(100);
}
