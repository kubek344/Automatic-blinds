#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PCF8574.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define Left 12
#define Right 14

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
PCF8574 pcf20(0x20);

unsigned int blinkMillis;
unsigned int buttonMillis;

int counter = 0;
int state;
int oldState;
int x = 0;
int y = 0;


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

void readEncoder(){
  state = digitalRead(Left);
  if(state != oldState){
    if(digitalRead(Right) != state){
      if(counter > 0){
        if(x % 2){
          counter--;
      }
        x++;
      }
    }else{
      if(counter < 9){
        if(y % 2){
          counter++;
        }
        y++;
      }
    }
    writeNumber(counter);
    target = (char)(counter+'0');
    Serial.println(target);
  }
  oldState=state; 
}

void beginDisplay(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    delay(200);
    beginDisplay();
  }
}

void writeNumber(int x) {
  display.clearDisplay();

  display.setTextSize(4);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(54,0);
  
  display.println(x);
  display.display();
  Serial.println(x);
}

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
Serial.println("Wait for clien");
readEncoder();
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

Serial.begin(9600);
pcf20.begin();
beginDisplay();

display.clearDisplay();
delay(2000);
writeNumber(0);
display.display();

pinMode(Left, INPUT);
pinMode(Right, INPUT);

oldState = digitalRead(Left);

forceApStart();

clientstatus();
Udp.begin(localUdpPort);
Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.softAPIP().toString().c_str(), localUdpPort);
server.begin();
}

void loop(){
readEncoder();

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

static bool state;
static bool statee;
static bool stateee;
unsigned int currentMillis = millis();
  
if(currentMillis - buttonMillis >= 50){
  buttonMillis = currentMillis;

  if(state != pcf20.readButton(0)){
    if(state){
      Serial.println("press 0");
      replyPacket = "00";
    }
      state = !state;
    }

  if(statee != pcf20.readButton(1)){
    if(statee){
      Serial.println("press 1");
      replyPacket = "01";
    }
    statee = !statee;
  }

  if(stateee != pcf20.readButton(2)){
    if(stateee){
      Serial.println("press 2");
      replyPacket = "02";
    }
  }
  stateee = !stateee;
}
  
if(currentMillis - blinkMillis >= 500){
  blinkMillis = currentMillis;

  pcf20.toggle(0);
  pcf20.toggle(1);
  pcf20.toggle(2);
  Serial.println(pcf20.read8(), BIN);
}

if(replyPacket != newreplyPacket){
    replyPacket[0] = target;
    Serial.println(replyPacket);
    Udp.beginPacket("192.168.4.255", localUdpPort);
    Udp.write(replyPacket);
    Udp.endPacket();
  }
}
