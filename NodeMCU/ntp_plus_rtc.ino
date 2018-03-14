#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "Wire.h"

const char *ssid     = "Yueverlasting";
const char *password = "10241228";
String cday[7]={"sun","mon","tue","wen","tur","fri","sat"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

#define DS3231_ADDRESS 0x68
/*
 * RTC
 */
void setDateTime(byte second, byte minute, byte hour, 
    byte weekDay, byte monthDay, byte month, byte year);

void setup(){
    /*WIFI部分*/
  Serial.begin(115200);
  Wire.begin();
  WiFi.begin(ssid, password);  //wifi開始連接

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();

}

void pf(const char *fmt, ...) {
  char tmp[128]; // max 128 chars
  va_list args;
  va_start(args, fmt);
  vsnprintf(tmp, 128, fmt, args);
  va_end(args);
  Serial.print(tmp);
}

void loop() {
  timeClient.update();
  
  Serial.println("NET time : ");
  print_netDate();

  Serial.println("RTC time : ");
  printDate();
  Serial.println("");
  delay(1000);
}


byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}
void printDate(){

  // Reset the register pointer
  Wire.beginTransmission(DS3231_ADDRESS);

  byte zero = 0x00;
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS3231_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());

  //print the date EG   11/3/1 23:59:59
  Serial.print(year);
  Serial.print("/");
  Serial.print(month);
  Serial.print("/");
  Serial.print(monthDay);
  Serial.print("  ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);
}
void print_netDate(){
  //print the date EG   3-1-11 23:59:59
  Serial.print( timeClient.getYear() );
  Serial.print("/");
  Serial.print(timeClient.getMonth());
  Serial.print("/");
  Serial.print(timeClient.getDate());
  Serial.print("  ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
}

void serialEvent()
{
  int type = Serial.read();
  int v = 0;
  int temp;
  while ((temp =  Serial.read()) > '0' - 1) 
  {
    pf("temp - 0 : %d\n", temp - '0');
    v = v * 10 + (temp - '0');
    pf("v : %d\n", v);
  }
  pf("type: %c\n", type);
  pf("v : %d\n", v);
  // -------------------------------
  // Reset the register pointer
  Wire.beginTransmission(DS3231_ADDRESS);

  byte zero = 0x00;
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS3231_ADDRESS, 7);

  byte s = bcdToDec(Wire.read());
  byte m = bcdToDec(Wire.read());
  byte h = bcdToDec(Wire.read() & 0b111111); //24 hour time
  byte weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  byte d = bcdToDec(Wire.read());
  byte M = bcdToDec(Wire.read());
  byte y = bcdToDec(Wire.read());
  
  switch(type) {
    case 'y':            y = v - 2000; break;
    case 'M':            M = v;        break;
    case 'd':            d = v;        break;
    case 'h':            h = v;        break;
    case 'm':            m = v;        break;
    case 's':            s = v;        break;
    default: Serial.println("Wrong\n");   break;
  }
 

  setDateTime(s, m, h, weekDay, d, M, y);
}

void setDateTime(byte second, byte minute, byte hour, 
    byte weekDay, byte monthDay, byte month, byte year) {
  byte zero = 0x00;
    
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write(zero); //stop Oscillator

  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(weekDay));
  Wire.write(decToBcd(monthDay));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));

  Wire.write(zero); //start 

  Wire.endTransmission();
}
