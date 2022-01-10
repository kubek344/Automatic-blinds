#include <Arduino.h>
#include "BasicStepperDriver.h"

#define MOTOR_STEPS 200
#define RPM 120

#define MICROSTEPS 1
#define DIR 5
#define STEP 16
#define SLEEP 13
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, SLEEP);

#define BLIND_LEN 10

int len = MOTOR_STEPS*MICROSTEPS*BLIND_LEN;
int halflen = len/2;
int halfblind = BLIND_LEN/2;
int rot = MOTOR_STEPS*MICROSTEPS;
int pos = 0;

void changePosition(int param){
  switch(param){
      case 0: //blind fully up
        stepper.move(-(len-rot*pos));
        pos=param*halfblind;
        break;

      case 1: //blind half down
        stepper.move(halflen-rot*pos);
        pos=param*halfblind;
        break;

      case 2: //blind fully down
        stepper.move(len-rot*pos);
        pos=param*halfblind;
        break;

      case 3: //blind down
      if(pos<=len){
        stepper.move(rot);
        pos+=1;
      }
        break;

      case 4: //blind up //lewo
      if(pos>0){
        stepper.move(-rot);
        pos-=1;
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
    pinMode(4, INPUT);  //red button
    pinMode(12, INPUT); //yellow button
    pinMode(14, INPUT); //green button
}

void loop() {
  byte down = digitalRead(4);
  byte mid = digitalRead(12);
  byte up = digitalRead(14);
  byte freee = digitalRead(0);
  Serial.printf("%x %x %x %x \n", down, mid, up, freee);
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
  stepper.move(200);
}
