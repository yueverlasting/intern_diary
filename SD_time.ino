#include <Wire.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <SD.h>

File myFile; 
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 28800);

const char *ssid     = "Yueverlasting";
const char *password = "10241228";
String cday[7]={"Sun","Mon","Tue","Wen","Tur","Fri","Sat"};
int ntime;
String YY,MM,Date,hh,mm,ss,wday;

void print_netDate(){
  //print the date EG   3-1-11 23:59:59
  Serial.print( timeClient.getYear() );
  Serial.print("/");
  Serial.print(timeClient.getMonth());
  Serial.print("/");
  Serial.print(timeClient.getDate());
  Serial.print("  ");
  Serial.print( (timeClient.getHours()) );
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.print(timeClient.getSeconds());
  Serial.print("  ");
  Serial.println( cday[timeClient.getDay()] );
}


void setup(){
    /*WIFI部分*/
  Serial.begin(57200);
  Serial.setTimeout(5000);
  Wire.begin(D4,D3);

  
  WiFi.begin(ssid, password);  //wifi開始連接
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

}
void day_set() {
  ntime = timeClient.getSeconds();
  YY = timeClient.getYear();
  MM = timeClient.getMonth();
  Date = timeClient.getDate();
  hh = timeClient.getHours();
  mm = timeClient.getMinutes();
  ss = timeClient.getSeconds();
  wday = cday[timeClient.getDay()];
}
void loop() {
  timeClient.update();
  while ( timeClient.getSeconds() != ntime){
    day_set();
    Serial.println("NET time : ");
    print_netDate();
    myFile = SD.open("test.txt", FILE_WRITE);
      // if the file opened okay, write to it:
    if (myFile) {
      Serial.print("Writing to test.txt...");
     myFile.println(YY+"/"+MM+"/"+Date+"\t"+hh+":"+mm+":"+ss);
      // close the file:
      myFile.close();
      Serial.println("done.");
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
    // re-open the file for reading:
    myFile = SD.open("test.txt");
    if (myFile) {
      Serial.println("ok");
      // read from the file until there's nothing else in it
      while (myFile.available()) {
        Serial.write(myFile.read());
      }
      // close the file:
      myFile.close();
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
  }
}




