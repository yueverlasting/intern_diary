# 在標頭檔 Adafruit_PN532.cpp 中
    
    #if defined(__AVR__) || defined(__i386__) //compatibility with Intel Galileo
    #define WIRE Wire
    #else // Arduino Due
    #define WIRE Wire1  =>  改成 #define WIRE Wire
    #endif
    
    註解掉   //SPI.setDataMode(SPI_MODE0);
    註解掉   //SPI.setBitOrder(LSBFIRST);
    註解掉   //SPI.setClockDivider(PN532_SPI_CLOCKDIV);
    增加       SPI.beginTransaction(SPISettings(7000000, MSBFIRST, SPI_MODE0));
    
