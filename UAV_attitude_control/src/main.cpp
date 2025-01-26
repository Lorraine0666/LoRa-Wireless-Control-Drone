#include "config.h"
#include "imu.h"
#include "port.h"
#include <Arduino.h>

using namespace std;

// command
String command_received;
String command_executed;
// attitude array
int attitude[3] = {-1, -1, -1};

void setup() {
  imu_setup();
  config();
}

void loop() {
  digitalWrite(LED1, HIGH);
  while (Serial1.available() > 0) {
    char temp_read = char(Serial1.read());
    command_received += temp_read;
  }
  Serial1.println(command_received);
}