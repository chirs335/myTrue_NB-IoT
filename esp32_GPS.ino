#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "cb9557f28e6e4efcad9255119b57e11f"; // You should get Auth Token in the Blynk App.
char ssid[] = "one+";      // ชื่อ wifi
char pass[] = "1212312121"; // รหัส
BlynkTimer timer;



#include <TinyGPS++.h>
#include <HardwareSerial.h>
#define RXPin (16)
#define TXPin (17)
TinyGPSPlus gps;
HardwareSerial ss(2);
static const uint32_t GPSBaud = 9600;

WidgetMap myMap(V0);
float spd;       //Variable  to store the speed
float sats;      //Variable to store no. of satellites response
String bearing;  //Variable to store orientation or direction of GPS
unsigned int move_index = 1;

void setup()
{
  
   Serial.begin(115200);
   Blynk.begin(auth, ssid, pass);
 
 // Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
 // Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
 
  ss.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin, false);
  Serial.println(TinyGPSPlus::libraryVersion());
   timer.setInterval(5000L, checkGPS);
}

void loop()
{
 while (ss.available() > 0) 
    {
      // sketch displays information every time a new sentence is correctly encoded.
      if (gps.encode(ss.read()))
        GPS();
  }


  
  Blynk.run();
  timer.run();
}

void GPS()
{
  if (gps.location.isValid() ) 
  {
    
    float latitude = (gps.location.lat());     //Storing the Lat. and Lon. 
    float longitude = (gps.location.lng()); 
    
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    Blynk.virtualWrite(V1, String(latitude, 6));   
    Blynk.virtualWrite(V2, String(longitude, 6));  
    myMap.location(move_index, latitude, longitude, "GPS_Location");
    spd = gps.speed.kmph();               //get speed
       Blynk.virtualWrite(V3, spd);
       
       sats = gps.satellites.value();    //get number of satellites
       Blynk.virtualWrite(V4, sats);

       bearing = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
       Blynk.virtualWrite(V5, bearing);               
  }
  else
  {
    Serial.println(F("INVALID"));
  }
  }

void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
      Blynk.virtualWrite(V4, "GPS ERROR");  // Value Display widget  on V4 if GPS not detected
  }
}
  
