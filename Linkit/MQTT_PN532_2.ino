#include <LWiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

// Update these with values suitable for your network.
const char* ssid = "TVTC-MOA";
const char* password = "22592566";
const char* mqtt_server = "192.168.64.113";

// If using the breakout with SPI, define the pins for SPI communication.
//定義腳位
#define PN532_SCK  (13)
#define PN532_MOSI (11)
#define PN532_SS   (10)
#define PN532_MISO (12)

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

uint8_t success;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;

//測試轉存
String uid_code ;

WiFiClient espClient;
PubSubClient client(espClient);
int value = 1;
String msg_1 ;

void setup() {
  Serial.begin(57600);

  nfc.begin();
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.println("Found chip PN532");
  // configure board to read RFID tags
  nfc.SAMConfig();
  Serial.println("Waiting for an ISO14443A Card ...");
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print ("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void nfc_check(){    
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    uid_code = ( nfc.PrintHex(uid, uidLength) );
    
    Serial.print("  UID code : ");
    Serial.println( uid_code );
    Serial.println("");

}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // ... and resubscribe 
      client.subscribe("Jade.h_Topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if ( WiFi.status() == false) {
      setup_wifi();}
  if (!client.connected()) {reconnect();}
    client.loop();
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    if (success) {
    nfc_check();  
    
    msg_1 = "UID #"; msg_1 +=  value ;msg_1 += " ";
    msg_1 +=  uid_code;
      
    Serial.print("Publish message: ");
    Serial.println(msg_1);
    client.publish("Jade.h_Topic",  msg_1.c_str() );
    value++ ;
    }
}
