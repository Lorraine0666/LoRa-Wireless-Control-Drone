#include "config.h"
#include "port.h"
#include "uav_motor.h"
#include <Arduino.h>

void setup() { config_setup(); }

void loop() {
  // use Euler angle to represent UAV's attitude
  // basic format: <y:0,r:0,p:0>
}