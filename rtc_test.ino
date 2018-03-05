#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>

// 簡便的工具函式，格式化輸出字串
void pf(const char *fmt, ... ){
    char tmp[128]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, fmt );
    vsnprintf(tmp, 128, fmt, args);
    va_end (args);
    Serial.print(tmp);
}

void setup()  {
  Serial.begin(115200);
  
  // 設定從哪裡取得時間，由DS1307RTC程式庫的RTC.get負責
  // 這也是此程式碼裡唯一使用DS1307RTC程式庫的地方
  // 其餘皆使用較高階的Time程式庫的介面
  setSyncProvider(RTC.get);

  // 判斷能否從DS1307取得時間 
  if(timeStatus() != timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");      
}

void loop()
{
  // 每隔一秒就輸出時間日期
   pf("%d/%d/%d %02d:%02d:%02d\n", year(), month(), day(), hour(), minute(), second()); 
   delay(1000);
}

// 聆聽序列埠，從這裡設定時間日期，格式如下：
// 「y2013」設定為2013年
// 「M1」設定為1月，注意是大寫的「M」
// 「d27」設定為26日
// 「h2」設定為2時
// 「m34」設定為34分
// 「s56」設定為56秒 
void serialEvent()
{
  int t = Serial.read();
  int v = 0;
  int temp;
  while((temp = Serial.read()) != -1){
    v = v * 10 + (temp - '0');
  }
  
  int y = year();
  int M = month();
  int d = day();
  int h = hour();
  int m = minute();
  int s = second();
  
  switch(t){
    case 'y':
      y = v;
      break;
    case 'M':
      M = v;
      break;
    case 'd':
      d = v;
      break;
    case 'h':
      h = v;
      break;
    case 'm':
      m = v;
      break;
    case 's':
      s = v;
      break;
    default:
      pf("Wrong\n");
      break;
  }

  tmElements_t tm;
  tm.Year = y - 1970; // 從1970年開始算
  tm.Month = M;
  tm.Day = d;
  tm.Hour = h;
  tm.Minute = m;
  tm.Second = s;
  // Time程式庫會有一段延遲時間，過後才會與RTC晶片同步
  // 所以在此強制把日期時間寫入RTC晶片 
  setTime(makeTime(tm));
  RTC.set(makeTime(tm)); 
}
