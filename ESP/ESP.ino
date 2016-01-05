 #include <ESP8266WiFi.h>
 
// replace with your channel's thingspeak API key,
String apiKey = "L7WIAKQ2NX51R786";
const char* ssid = "Fadh";
const char* password = "12345678";
 
const char* server = "api.thingspeak.com";

WiFiClient client;

String pulse;

int timer = 0;
void setup() {
  Serial.begin(115200);
  delay(10);
  WiFi.begin(ssid, password);
}
 
void loop() {
  if (Serial.available() > 0)
    pulse = Serial.readStringUntil('\n');

  if (pulse != 0){
  if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(pulse);
 
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);
  }
  client.stop();
  Serial.println(pulse);
  }
delay(20000);
}
