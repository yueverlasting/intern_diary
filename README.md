# intern_diary

開始時間 : 180301

3/6 => NTP抓網路時間 & RTC時間同時顯示
       修改NTPClient 函式庫標頭檔

       標頭檔修改參考 : https://github.com/arduino-libraries/NTPClient/issues/36

3/7 => RTC網路校時 LCD測試 linkit 開發板安裝

       ntp_lcd.ino   -> LCD顯示 NTP時間
       
       3IN1.ino      -> LCD顯示 NTP時間 + RTC
                        腳位 SDA -> D2 , SCL ->D1

3/8 => linkit 7697 studying
       PN532 整個GG 

3/13 => PN532 成功 & sd card測試 & MQTT基礎閱讀

        嘗試 PN532 + LCD =>失敗
        sd card測試第一階段 確認連結上nodemcu
        明天帶sd卡測試
        
3/14 => SD卡測試 & 將RFID的資訊存入SD CARD
