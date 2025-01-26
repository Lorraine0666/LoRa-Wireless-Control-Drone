#include "port.h"
#include <Arduino.h>

int motor[] = {PWM1, PWM2, PWM3, PWM4};

int speed[] = {-1, -1, -1, -1};

void motor_set_speed() {
  for (int i = 0; i < 4; i++) {
    analogWrite(motor[i], speed[i]);
  }
}