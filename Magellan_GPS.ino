#include "Magellan.h"
#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <Wire.h>
Magellan magel;
char auth[]="dd13f7a0-df93-11e8-8443-17f06f0c0a93"; 		//Token Key you can get from magellan platform
String payload;

SoftwareSerial serial_connection(10, 11); //RX=pin 10, TX=pin 11
TinyGPSPlus gps;

String Lat,LON,Location;
void setup() {
  Serial.begin(9600);
  magel.begin(auth);           //init Magellan LIB
  serial_connection.begin(9600);
}

void loop() {
  
while(serial_connection.available())
{
gps.encode(serial_connection.read());
}
if(gps.location.isUpdated())
{

 Lat=String(gps.location.lat(), 6);
  LON=String(gps.location.lng(), 6);
}
   Location = Lat+","+LON;
  payload="{\"Location\":"+Location+"}";       //please provide payload with json format

  magel.post(payload);                            							   //post payload data to Magellan IoT platform
  
}
