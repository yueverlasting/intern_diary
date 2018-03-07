#include <DS3231.h>
#include <Wire.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

DS3231 Clock;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 28800);

const char *ssid     = "Yueverlasting";
const char *password = "10241228";
String cday[7]={"sun","mon","tue","wen","tur","fri","sat"};
String str_Ntime, str_Rtime;

byte Year;
byte Month;
byte Date;
byte DoW;
byte Hour;
byte Minute;
byte Second;

bool Century = false;
bool h12;
bool PM;

bool GetDateStuff(byte& Year, byte& Month, byte& Day, byte& DoW,
                  byte& Hour, byte& Minute, byte& Second) {
  // Call this if you notice something coming in on
  // the serial port. The stuff coming in should be in
  // the order YYMMDDwHHMMSS, with an 'x' at the end.

  byte Temp1, Temp2;
  char InString[20];

  size_t expectLen = sizeof("YYMMDDwHHMMSS") / sizeof(char) - 1;
  size_t actualLen = Serial.readBytesUntil('x', InString, sizeof(InString) / sizeof(char));
  // Serial.printf("%d %d\n", actualLen, expectLen);
  if (actualLen != expectLen) return false;

  // Read Year first
  Temp1 = (byte)InString[0] - 48;
  Temp2 = (byte)InString[1] - 48;
  Year = Temp1 * 10 + Temp2;
  // now month
  Temp1 = (byte)InString[2] - 48;
  Temp2 = (byte)InString[3] - 48;
  Month = Temp1 * 10 + Temp2;
  // now date
  Temp1 = (byte)InString[4] - 48;
  Temp2 = (byte)InString[5] - 48;
  Day = Temp1 * 10 + Temp2;
  // now Day of Week
  DoW = (byte)InString[6] - 48;
  // now Hour
  Temp1 = (byte)InString[7] - 48;
  Temp2 = (byte)InString[8] - 48;
  Hour = Temp1 * 10 + Temp2;
  // now Minute
  Temp1 = (byte)InString[9] - 48;
  Temp2 = (byte)InString[10] - 48;
  Minute = Temp1 * 10 + Temp2;
  // now Second
  Temp1 = (byte)InString[11] - 48;
  Temp2 = (byte)InString[12] - 48;
  Second = Temp1 * 10 + Temp2;

  return true;
}

void GetDateStuffFromNtpClient(byte& Year, byte& Month, byte& Day, byte& DoW,
                               byte& Hour, byte& Minute, byte& Second) {
  Year = Clock.getYear();
  Month = Clock.getMonth();
  Day = Clock.getDate();
  DoW = (byte)timeClient.getDay();
  Hour = (byte)timeClient.getHours();
  Minute = (byte)timeClient.getMinutes();
  Second = (byte)timeClient.getSeconds();
}

void setup(){
    /*WIFI部分*/
  Serial.begin(115200);
  Serial.setTimeout(5000);
  Wire.begin();
  WiFi.begin(ssid, password);  //wifi開始連接

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();

}

void loop() {
  timeClient.update();
  
  Serial.println("NET time : ");
  print_netDate();

  Serial.println("RTC time : ");
  printDate();
  Serial.println("");
  
  setCurrentTime(Year, Month, Date, DoW, Hour, Minute, Second);
    if (Serial.available()) {
    bool success = GetDateStuff(Year, Month, Date, DoW, Hour, Minute, Second);
    if (success) {
      Clock.setClockMode(false);  // set to 24h
      //setClockMode(true); // set to 12h

      setCurrentTime(Year, Month, Date, DoW, Hour, Minute, Second);
    }
  }
  delay(1000);
}

void printDate(){
  Serial.print(Clock.getYear(), DEC);
  Serial.print("-");
  Serial.print(Clock.getMonth(Century), DEC);
  Serial.print("-");
  Serial.print(Clock.getDate(), DEC);
  Serial.print(" ");
  Serial.print(Clock.getHour(h12, PM), DEC); //24-hr
  Serial.print(":");
  Serial.print(Clock.getMinute(), DEC);
  Serial.print(":");
  Serial.println(Clock.getSecond(), DEC);
}
void print_netDate(){
  str_Ntime = ((timeClient.getHours())+8) +(timeClient.getMinutes()) + (timeClient.getSeconds()) ;
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

void setCurrentTime(byte& Year, byte& Month, byte& Day, byte& DoW,
                    byte& Hour, byte& Minute, byte& Second) {
  Clock.setYear(Year);
  Clock.setMonth(Month);
  Clock.setDate(Date);
  Clock.setDoW(DoW);
  Clock.setHour(Hour);
  Clock.setMinute(Minute);
  Clock.setSecond(Second);
}
