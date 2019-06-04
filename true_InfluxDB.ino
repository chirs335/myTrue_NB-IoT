#include <TinyGPS++.h> // Include the TinyGPS++ library
TinyGPSPlus tinyGPS; // Create a TinyGPSPlus object

#include <SoftwareSerial.h> // Include SoftwareSerial library for GPS
static const int RXPin = 4, TXPin = 3; // GPS TX, Arduino RX pin GPS RX, Arduino TX pin
static const uint32_t GPSBaud = 9600; // GPS module baud rate. GP3906 defaults to 9600.
 
SoftwareSerial ssGPS(RXPin, TXPin); // Create a SoftwareSerial

#define SerialMonitor Serial // Define the serial monitor port.

#include <True_NB_bc95.h> // Include NB-IoT Module library
#include <AltSoftSerial.h> // Include AltSoftSerial library for NB-IoT Module

AltSoftSerial asSerial; // Create a AltSoftSerial object
True_NB_bc95 bc95; // Create a True_NB_bc95 object
String db_host="xx.xx.xx.xx"; // IP UDP Host
String db_port="xxxx"; // UDP Port
char sock[] = "0\0";

#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#include <geohash.h>

void getGPSInfo();
void getDHT();
void gpsHash();

String lat,lng,humi,temp,ghash,ss,data;
uint8_t dataValid = 0;

void setup()
{
  Serial.begin(9600);
  asSerial.begin(9600);
  Serial.println("Starting...");
  delay(3000);
  bc95.init(asSerial);
  bc95.initModem();
  delay(1000);
  while (!bc95.register_network());
  bc95.create_UDP_socket(4587, sock);
  delay(1000);
}

void loop()
{
  getGPSInfo();
  getDHT();

  if (dataValid == 1){
    ss = String(bc95.check_modem_signal());
    data= String("station3,geohash="+ ghash + ",country=TH lat="+ lat + ",lng=" + lng 
    + ",humi="+ humi + ",temp=" + temp + ",signal=" + ss);  
    Serial.println('.');    
    Serial.println(data);  
    Serial.println("Send..");
    bc95.sendUDPstr(db_host,db_port,data);
    dataValid = 0;  
  }else{
    Serial.print('.'); 
  }
  delay(1000);

}

void getGPSInfo()
{  
  ssGPS.begin(GPSBaud);
  dataValid = 0;
  while (ssGPS.available() > 0){
    if (tinyGPS.encode(ssGPS.read())){
      if(tinyGPS.location.isValid()){
        Serial.print('|');
        lat = String(tinyGPS.location.lat(), 6);
        lng = String(tinyGPS.location.lng(), 6);
        gpsHash();
        dataValid = 1;
        ssGPS.end();
      }
      else
      {
        Serial.println(F("INVALID!"));
        dataValid = 0;
        //ssGPS.end();
      }
    }
  } 
}

void getDHT()
{
  humi = String(dht.readHumidity(),2);
  temp = String(dht.readTemperature(),2);
  if (!humi.length()||!temp.length())
  {
    Serial.println("Failed to read from DHT sensor!");
  }else{
    Serial.print('*');
  }
}

void gpsHash()
{
  char* gpshash = geohash_encode(lat.toDouble(), lng.toDouble(), 9);
  ghash = String(gpshash);
  if(!ghash.length())
  {
    Serial.println("geoHash somthing wrong!");
  }else{
    Serial.print('-');
  }
}
