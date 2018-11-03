#include <Arduino.h>
// Nb-IOT 
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)      // Arduino UNO
    #include <AltSoftSerial.h>
    AltSoftSerial bc95serial;
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)   // Arduino MEGA2560
    #define bc95serial Serial1
#endif

#include "BC95Udp.h"
#include "MicrogearNB.h"

#define APPID    "APPID"
#define KEY      "KEY"
#define SECRET   "SECRET"
#define FEEDID "FEEDID"
BC95UDP client;
Microgear mg(&client);
/////////////////////////////////////////////////////////////////////////////////////////////

#include "DHT.h"
DHT dht;
#define MAXBUFFSIZE 32
//////////////////////////////////////////////////////////////////////////////////////
char payload[32];
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

    dht.setup(2);
    
}

void loop() {

float humidity = dht.getHumidity(); // อ่านค่าความชื้น
float temperature = dht.getTemperature(); // อ่านค่าอุณหภูมิ


Serial.print("Humid: "); Serial.print(humidity); Serial.print(" %, ");
Serial.print("Temp: "); Serial.print(temperature); Serial.println(" C ");
// ตรวจสอบค่า humid และ temp เป็นตัวเลขหรือไม่
if (isnan(humidity) || isnan(temperature)) {
Serial.println("Failed to read from DHT sensor!");
}
else {
// เตรียมสตริงในรูปแบบ "humid,temp"
String datastring = (String)humidity + "," + (String)temperature;


Serial.print(F("Sending --> "));
Serial.println(datastring);
datastring.toCharArray(payload, MAXBUFFSIZE-1);
mg.publish("/nbiot/sensor", payload);
mg.loop();
/////////////////////////////////////////////////////////////////////////////////

String data = "humid:";   
data += humidity ;       
data += ",temp:";
data += temperature;
data.toCharArray(payload,MAXBUFFSIZE-1);

Serial.print(F("feed --> "));
Serial.println(payload);
 mg.writeFeed("Temptime",payload); 

mg.loop();
}

Serial.print(F("Sent Signal Strength: "));
Serial.println(BC95.getSignalStrength());
mg.publish("/nbiot/rssi", BC95.getSignalStrength());


delay(5000);
mg.loop();

}
