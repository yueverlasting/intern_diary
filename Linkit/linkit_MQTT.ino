#include <LWiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "Yueverlasting";
const char* password = "10241228";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
String msg_1 ;

void setup() {
  Serial.begin(57600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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
  Serial.println("IP address: ");
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

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    if( value >20 )
    { //snprintf (msg, 75, "hello world #%ld", value);
      msg_1 = "hello world #";
      msg_1 += value ;
      }
    else 
    {//snprintf (msg, 75, "hi Jade #%ld", value);
      msg_1 = "hello world #";
      msg_1 += value ;
    }
    //Serial.print("Publish message: ");
    //Serial.println(msg);
    Serial.print("msg_1 : ");
    Serial.println(msg_1);
    client.publish("Jade.h_Topic",  msg_1.c_str() );
  }
}
