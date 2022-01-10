#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "scarlet";
const char* password = "";

void forceApConn(){
  if (WiFi.status() == WL_CONNECTED){
     return;
  }

  Serial.print("Connecting");
  WiFi.begin( ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets

void setup(){
  Serial.begin(9600);
  Serial.println();

  
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);

  forceApConn();

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

void loop(){
  forceApConn();
  int packetSize = Udp.parsePacket();
  if (packetSize){
   int len = Udp.read(incomingPacket, 255);
   if (len > 0){
      incomingPacket[len] = '\0';
      Serial.println(incomingPacket);
      switch(incomingPacket[0]){
          case '0':
            digitalWrite(12, HIGH);
            digitalWrite(13, LOW);
            digitalWrite(14, LOW);
            break;
          case '1':
            digitalWrite(12, LOW);
            digitalWrite(13, HIGH);
            digitalWrite(14, LOW);
            break;
          case '2':
            digitalWrite(12, LOW);
            digitalWrite(13, LOW);
            digitalWrite(14, HIGH);
            break;
        }
    }
  }
}
