    ฝั่งส่ง                                      

void setup()
{
Serial.begin(9600);
porttran.begin(9600);
}

void loop()
{
porttran.write("#");
porttran.print((103.532512),6);
porttran.write(":");
porttran.print(((505.1257952)),6);
porttran.write(";");
porttran.println();

Serial.write("#");
Serial.print((103.532512),6);
Serial.write(":");
Serial.print(((503.1257952)),6);
Serial.write(";");
Serial.println();


delay(2000);
  }
  
  
  
ฝั่งรับ   
  
  
  #include "SoftwareSerial.h"
SoftwareSerial porttran(2, 3); 

void setup() {
 porttran.begin(9600);
 Serial.begin(9600);

}
void loop() {
  static int state = 0;
  static int index = 0;
  static char BUFFER[12];
  static float val1,val2;
  porttran.listen();
  while(porttran.available())
{
  char data = porttran.read();
  switch(state){
    case 0: if(data == '#'){
                index = 0;
                state =1;           
                }
    break;
    
    case 1: if(data == ':'){
                
                val1 = atof(BUFFER);
                index = 0;
                state =2;
                
                }
            else{
              BUFFER[index]=data;
              index++;
                
            }
   break;
    case 2: if(data == ';'){
                
                val2 = atof(BUFFER);
                state =0;
                Serial.print("val1=");
                 Serial.print(val1,6);
                 Serial.print("val2=");
                Serial.println(val2,6);              
                }
            else{
              BUFFER[index]=data;
              index++;
              
            }
   break;       
  }
}
}
  
  
  
  
  
  
  
