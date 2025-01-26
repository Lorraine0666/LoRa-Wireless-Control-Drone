#include "config.h"
#include "port.h"
#include <Arduino.h>

void config() {
  // init motor pin
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);

  // init battery pin
  pinMode(ADC_BATTERY, INPUT_ANALOG);

  // init LED1
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // init serial1
  Serial1.begin(115200);
}