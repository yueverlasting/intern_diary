# intern_diary

開始時間 : 180301

3/6 => NTP抓網路時間 & RTC時間同時顯示
       修改NTPClient 函式庫標頭檔

       標頭檔修改參考 : https://github.com/arduino-libraries/NTPClient/issues/36

3/7 => RTC網路校時 LCD測試 linkit 開發板安裝

       檔案: ntp_lcd.ino
       功能: LCD顯示 NTP時間
       
       檔案: 3IN1.ino
       功能: LCD顯示 NTP時間 + RTC
       
       腳位 SDA -> D2 , SCL ->D1

3/8 => linkit 7697 studying & PN532 整個GG 

3/13 => PN532 成功 & sd card測試 & MQTT基礎閱讀
                   
        檔案: NFC_READ.INO
        功能: 讀取NFC卡片
        腳位:  NFC	 NodeMCU
              SS (CS) -> D2	(GPIO4) 
              IRQ     ->	D3	(GPIO0)
              RST	    -> D4	(GPIO2) 
              SCK     ->	D5	(GPIO14)
              MISO    -> D6	(GPIO12)
              MOSI    -> D7	(GPIO13)
              

3/14 => SD卡測試 & 將RFID的資訊存入SD CARD & 測試linkit7697

       檔案 : SD_TEST.INO
       功能 : 在SD卡的TEST.TXT 寫入現在時間
       腳位 :  CS    ->D8
               MOSI -> D7
               SCK  -> D5
               MISO -> D6
       
3/15 => NodeMCU 監聽模式

       檔案 : spiffy6.ino
       功能 : 接收所有mac address
       
