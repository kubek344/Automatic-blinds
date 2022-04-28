#include <Arduino.h>
#include "BasicStepperDriver.h"

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
        stepper.move(-200);
        pos--;
      }
      break;

    case '1':
      while (pos > maxRots / 2) {
        stepper.move(-200);
        pos--;
      }
      while (pos < maxRots / 2) {
        stepper.move(200);
        pos++;
      }
      break;

    case '2':
      while (pos < maxRots) {
        stepper.move(200);
        pos++;
      }
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

void setup() {
  Serial.begin(9600);
  stepper.begin(RPM, MICROSTEPS);
}

void loop() {
  changePosition('3');
  changePosition('0');
  changePosition('2');
  changePosition('1');
  changePosition('2');
  changePosition('4');
  delay(1000);
}
