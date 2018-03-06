#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid     = "Yueverlasting";
const char *password = "10241228";

String cday[7]={"sun","mon","tue","wen","tur","fri","sat"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup(){
    /*WIFI部分*/
  Serial.begin(115200);
  WiFi.begin(ssid, password);  //wifi開始連接

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();

}

void loop() {
  timeClient.update();
  
  Serial.println(cday[(timeClient.getDay())]);
  Serial.println(timeClient.getFormattedTime() +"\n");

  delay(1000);
}
 
