#include <Arduino.h>
#include "BasicStepperDriver.h"

#define MOTOR_STEPS 200
#define RPM 120

#define MICROSTEPS 1
#define DIR 5
#define STEP 16
#define SLEEP 13
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, SLEEP);

#define BLIND_LEN 1

int len = MOTOR_STEPS*MICROSTEPS*BLIND_LEN;
int pos = 0;

void changePosition(int param){
  switch(param){
      case 0:
        switch(pos){
            case 1: //blind fully up
              stepper.move(len/2);
              break;
            case 2:
              stepper.move(len);
              break;
            default:
              break;
          }
        break;

      case 1: //blind half down
        switch(pos){
            case 0:
              stepper.move(len/2*-1);
              break;
            case 2:
              stepper.move(len/2);
              break;
            default:
              break;
          }
        break;

      case 2: //blind fully down
        switch(pos){
            case 0:
              stepper.move(len*-1);
              break;
            case 1:
              stepper.move(len/2*-1);
              break;
            default:
              break;
          }
        break;
      default:
        break;
    }
    pos=param;
}


void setup() {
    Serial.begin(9600);
    stepper.begin(RPM, MICROSTEPS);
    pinMode(0, INPUT_PULLUP);
    pinMode(4, INPUT);  //red button
    pinMode(12, INPUT); //yellow button
    pinMode(14, INPUT); //green button
}

void loop() {
  byte down = digitalRead(4);
  byte mid = digitalRead(12);
  byte up = digitalRead(14);
  byte freee = digitalRead(0);
  if(freee == LOW){
    if(digitalRead(SLEEP) == LOW){
      stepper.enable();
    }else{
      stepper.disable();
    }
  }
  if(down == HIGH){
    changePosition(2);
  }
  if(mid == HIGH){
    changePosition(1);
  }
  if(up == HIGH){
    changePosition(0);
  }
  delay(500);
}
