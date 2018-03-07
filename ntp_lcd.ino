#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 28800);

const char *ssid     = "Yueverlasting";
const char *password = "10241228";
String cday[7]={"sun","mon","tue","wen","tur","fri","sat"};
char*  Ntime ;

LiquidCrystal_I2C lcd(0x27, 20, 4);

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

  lcd.begin();       // Should match the number provided to the constructor.
  lcd.backlight();     // Turn on the backlight.
  //lcd.print("hello,maker"); 
  
  WiFi.begin(ssid, password);  //wifi開始連接
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();

}

void lcd_play() {
 lcd.setCursor(0,0);
  lcd.print(timeClient.getYear());
  lcd.print("/");
  lcd.print(timeClient.getMonth());
  lcd.print("/");
  lcd.print(timeClient.getDate());
  lcd.print(" ");
  lcd.print( cday[timeClient.getDay()] );
  
  lcd.setCursor(0,1);  // (行,列)
  lcd.print( (timeClient.getHours()) );
  lcd.print(":");
  
  if(timeClient.getMinutes() <10)
  {lcd.print("0");}
  lcd.print(timeClient.getMinutes());
  lcd.print(":");
  
  if(timeClient.getSeconds() <10)
  {lcd.print("0");}
  lcd.print(timeClient.getSeconds());
}

void loop() {
  timeClient.update();
  lcd_play();
  Serial.println("NET time : ");
  print_netDate();
  delay(1000);
}

