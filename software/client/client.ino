#include <Arduino.h>
#include "BasicStepperDriver.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "scarlet";
const char* password = "";

const char id = '1';

#define MOTOR_STEPS 200
#define RPM 120

#define MICROSTEPS 1
#define DIR 5
#define STEP 13
#define SLEEP 16
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, SLEEP);

int maxRots = 10;
int pos = maxRots;

void changePosition(char param) {
  switch (param) {
    case '0':
      while (pos > 0) {
        stepper.move(-360);
        pos--;
        Serial.println(pos);
      }
      break;

    case '1':
      while (pos > maxRots / 2) {
        stepper.move(-360);
        pos--;
        Serial.println(pos);
      }
      while (pos < maxRots / 2) {
        stepper.move(360);
        pos++;
        Serial.println(pos);
      }
      break;

    case '2':
      while (pos < maxRots) {
        stepper.move(360);
        pos++;
        Serial.println(pos);
      }
      Serial.println(pos);
      break;

    case '3':
      stepper.enable();
      break;

    case '4':
      stepper.disable();
      break;

    default:
      break;
  }
}

void forceApConn() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.print("Connecting");
  WiFi.begin( ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
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

void setup() {
  Serial.begin(9600);

  forceApConn();

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

void loop() {
  forceApConn();
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = '\0';
      Serial.println(incomingPacket);
      if (id == incomingPacket[0] || '0' == incomingPacket[0]) {
        /*switch (incomingPacket[1]) {
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
        }*/
        changePosition(incomingPacket[1]);
      }
    }
  }
}
