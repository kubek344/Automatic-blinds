#include <Arduino.h>
#include "BasicStepperDriver.h"

#define MOTOR_STEPS 200
#define RPM 120

#define MICROSTEPS 1
#define DIR 5
#define STEP 16
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

#define BLIND_LEN 5

int len = MOTOR_STEPS*MICROSTEPS*BLIND_LEN;
int pos = 0;
int o=1;


void changePosition(int param){

  switch(param){
      case 0:
        switch(pos){
            case 1: //blind fully up
              stepper.rotate(len/2);
              pos=param;
              break;
            case 2:
              stepper.rotate(len);
              pos=param;
              break;
            default:
              pos=param;
              break;
          }
        break;

      case 1: //blind half down
        switch(pos){
            case 0:
              stepper.rotate(len/2);
              pos=param;
              break;
            case 2:
              stepper.rotate(-len/2);
              pos=param;
              break;
            default:
              pos=param;
              break;
          }
        break;

      case 2: //blind fully down
        switch(pos){
            case 0:
              stepper.rotate(-len);
              pos=param;
              break;
            case 1:
              stepper.rotate(-len/2);
              pos=param;
              break;
            default:
              pos=param;
              break;
          }
        break;
      
      default:
        break;        
    }
}


void setup() {
    Serial.begin(9600);
    stepper.begin(RPM, MICROSTEPS);
    pinMode(0, INPUT_PULLUP);
}

void loop() {
  //changePosition(0);
  //changePosition(1);
  //changePosition(2); 
  delay(500);
}
