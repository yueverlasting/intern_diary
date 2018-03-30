int i=0;
void setup() {
  Serial.begin(57600);
}
void loop() {
  if (Serial.available() > 0) {
    i=Serial.read();
    
    Serial.write(i); 
  }
}
