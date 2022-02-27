#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "scarlet";
const char* password = "";

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

WiFiUDP Udp;
unsigned int localUdpPort = 4210; // local port to listen on
char incomingPacket[255]; // buffer for incoming packets
char* replyPacket = "03"; // a reply string to send back
String line = "";
char target = '0';

WiFiServer server(80);



void clientstatus(){
  unsigned char number_client;
  struct station_info *stat_info;

  struct ip_addr *IPaddress;
  IPAddress address;
  int i=1;

  number_client= wifi_softap_get_station_num();
  stat_info = wifi_softap_get_station_info();

  Serial.print(" Total Connected Clients are = ");
  Serial.println(number_client);

  delay(500);
}

void forceApStart(){
  
Serial.print("Setting soft-AP configuration ... ");
Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

Serial.print("Setting soft-AP ... ");
Serial.println(WiFi.softAP(ssid,password) ? "Ready" : "Failed!");

Serial.print("Soft-AP IP address = ");
Serial.println(WiFi.softAPIP());

while (WiFi.softAPgetStationNum()== 0)
{
delay(1000);
Serial.println("Wait for clien");
}

Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
Serial.print("Soft-AP IP address = ");
Serial.println(WiFi.softAPIP());
Serial.printf("MAC address = %s\n", WiFi.softAPmacAddress().c_str());



  Serial.print( "Listen to TCP port:");

  Serial.println( server.port());

  server.begin();
}

void setup()
{
Serial.begin(9600);
Serial.println();

pinMode(12, INPUT); //yellow button
pinMode(13, INPUT); //yellow button
pinMode(14, INPUT); //green button

forceApStart();

clientstatus();
Udp.begin(localUdpPort);
Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.softAPIP().toString().c_str(), localUdpPort);
server.begin();
}

void loop(){
  
char* newreplyPacket = replyPacket;
 WiFiClient client = server.available();// wait for a client (web browser) to connect
  if (client){
    Serial.println("\n[Client connected]");
    if( client.connected()){

      

        
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("<a href=\"/button/a\"><div style=\"float: left; height: 10vh; width: 30vw; background-color: black;\"></div>");
      client.println("<div style=\"float: left; height: 30vh; width: 3vw;\"></div>");
      client.println("<a href=\"/button/b\"><div style=\"float: left; height: 50vh; width: 30vw; background-color: black;\"></div>");
      client.println("<div style=\"float: left; height: 30vh; width: 3vw;\"></div>");
      client.println("<a href=\"/button/c\"><div style=\"float: left; height: 90vh; width: 30vw; background-color: black;\"></div>");
      
      if( !client.available()){ delay(150);}
      if( !client.available()){ delay(3000);}
      if (client.available()){
        
        String line = client.readStringUntil('\n');
        Serial.println( line);

        if(line.indexOf("/button/a") >= 0){
          Serial.println("Button A");
          replyPacket = "00";
        }else if(line.indexOf("/button/b") >= 0){
          Serial.println("Button B");
          replyPacket = "01";
        }else if(line.indexOf("/button/c") >= 0){
          Serial.println("Button C");
          replyPacket = "02";
        }else{
          Serial.println("huh");
          }
          replyPacket[0] = target;

        client.flush();
      }
      client.stop();
      Serial.println("[Client disconnected]"); 
    }
  }

 
    int packetSize = Udp.parsePacket();
    
    if(packetSize){
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
      if (len > 0)
      {
      incomingPacket[len] = 0;
      }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    }
byte a = digitalRead(12);
byte b = digitalRead(13);
byte c = digitalRead(14);
if(a){
  replyPacket = "00";
}else if(b){
  replyPacket = "01";
}else if(c){
  replyPacket = "02";
}
if(replyPacket != newreplyPacket){
    replyPacket[0] = target;
    Serial.println(replyPacket);
    Udp.beginPacket("192.168.4.255", localUdpPort);
    Udp.write(replyPacket);
    Udp.endPacket();
  }
}
