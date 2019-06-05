#include <Arduino.h>
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)      // Arduino UNO
#include <AltSoftSerial.h>
AltSoftSerial bc95serial;
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)   // Arduino MEGA2560
#define bc95serial Serial1
#endif
#include "BC95Udp.h"
#include "MicrogearNB.h"
#define APPID    "ChirsNBIoT"
#define KEY      "lWwr32z9U9aE5Ok"
#define SECRET   "GnJIp2qGqXDTNgv1TMKsH7zxi"
#define FEEDID "Temptime"
BC95UDP client;
Microgear mg(&client);
#define MAXBUFFSIZE 32
char payload[32];
/////////////////////////////////////
#include "TinyGPS++.h"
#include <SoftwareSerial.h>

SoftwareSerial serial_connection(4, 3); //
TinyGPSPlus gps;
String LAT,LNG,ghash,ss,data;
uint8_t dataValid = 0;



void(* resetFunc) (void) = 0;
void setup() {
    bc95serial.begin(9600);
    BC95.begin(bc95serial);
    BC95.reset();
    Serial.begin(9600);
    Serial.println(F("Microgear Arduino NB-IoT Start!"));
    Serial.print(F("IMEI: "));
    Serial.println(BC95.getIMEI());
    Serial.print(F("IMSI: "));
    Serial.println(BC95.getIMSI());

    Serial.print(F("Attach Network..."));
    while (!BC95.attachNetwork()) {
    Serial.print(".");
    delay(1000);
    }
    Serial.println(F("\nNB-IOT attached!"));
    Serial.print(F("RSSI: "));
    Serial.println(BC95.getSignalStrength());
    Serial.print(F("IPAddress: "));
    Serial.println(BC95.getIPAddress());
    mg.init(APPID, KEY, SECRET);
    mg.begin(5555);
    delay(2000);
    serial_connection.begin(9600);
}

void loop() {

   getGPSInfo();
   sentdata();
}



void getGPSInfo()
{  
  serial_connection.begin(9600);
  dataValid = 0;
  while (serial_connection.available()>0){
  //  if (tinyGPS.encode(ssGPS.read())){
        gps.encode(serial_connection.read()) ;
         }
         if(gps.location.isValid()){
         if(gps.location.isUpdated()){   
        Serial.print('|');
        LAT = String(gps.location.lat(), 6);
        LNG = String(gps.location.lng(), 6);
      //  gpsHash();
        dataValid = 1;
       Serial.print("LAT: "); Serial.print(LAT); 
        Serial.print("LNG: "); Serial.print(LNG); Serial.println("");

        serial_connection.end();
          delay(5000);
        
      }
      else
      {
        Serial.println(F("."));
        dataValid = 0;
        delay(100);
      }
         }
    
 
}



void sentdata()
{
  
  
  if (dataValid == 1){
    delay(2000);
    String datastring = (String)LAT + "," + (String)LNG;
    Serial.print(F("Sending --> "));
    Serial.println(datastring);
    datastring.toCharArray(payload, MAXBUFFSIZE-1);
    mg.publish("/nbiot/sensor", payload);
    
/////////////////////////////////////////////////////////////////////////////////

/*
String data = "humid:";   
data += LAT ;       
data += ",temp:";
data += LNG;
data.toCharArray(payload,MAXBUFFSIZE-1);

Serial.print(F("feed --> "));
Serial.println(payload);
mg.writeFeed("Temptime",payload); 
mg.loop();

*/

    Serial.print(F("Sent Signal Strength: "));
    Serial.println(BC95.getSignalStrength());
    mg.publish("/nbiot/rssi", BC95.getSignalStrength());
    delay(500);
    dataValid = 0;  
    mg.loop();
    resetFunc();


    
}
else{
    Serial.println("nonsent"); 
    delay(100);
  }
}

  
