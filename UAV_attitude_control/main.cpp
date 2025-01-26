#include "imu.h"
#include "port.h"
#include "test.h"
#include <Arduino.h>
#include <LoRa.h>
#include <MPU6500_WE.h>
#include <Wire.h>
#include <string>

using namespace std;

extern float roll;
extern float pitch;

// command for drone control
// basic format: <yaw:0,roll:0,pitch:0>
string command_received;
string command_execute;
int attitude[3] = {-1, -1, -1};

void setup() {
  test_setup();
  imu_setup();
}

void loop() {
  // check serial
  while (Serial1.available() > 0) {
    // read command
    char temp = char(Serial1.read());
    command_received += temp;
  }

  if (command_received != "") {
    command_execute = command_received;
  }

  // Debug: Print the current command
  Serial1.print("Received command: ");
  Serial1.println(command_received.c_str());

  if (command_execute.length() > 0) {
    size_t begin = command_execute.find('<');
    size_t end = command_execute.find('>', begin);

    if (begin != std::string::npos && end != std::string::npos && begin < end) {
      // Extract the portion between '<' and '>'
      std::string command_substr =
          command_execute.substr(begin, end - begin + 1); // Extract substring
      // Now parse the values
      int result = sscanf(command_substr.c_str(), "<yaw:%d,roll:%d,pitch:%d>",
                          &attitude[0], &attitude[1], &attitude[2]);

      // Debug: Print parsed attitude
      if (result == 3) {
        Serial1.print("Parsed attitude: ");
        Serial1.print("yaw = ");
        Serial1.print(attitude[0]);
        Serial1.print(", roll = ");
        Serial1.print(attitude[1]);
        Serial1.print(", pitch = ");
        Serial1.println(attitude[2]);
      } else {
        Serial1.println("Error parsing command");
      }
    }
  }

  // set the voltage of each motor according to attitude command
  if (attitude[0] == 0 && attitude[1] == 0 && attitude[2] == 0) {
    analogWrite(PWM1, 0);
    analogWrite(PWM2, 0);
    analogWrite(PWM3, 0);
    analogWrite(PWM4, 0);
  } else {
    // default mode: keep still
    analogWrite(PWM1, 0);
    analogWrite(PWM2, 0);
    analogWrite(PWM3, 0);
    analogWrite(PWM4, 0);
  }

  // Don't reset command here, keep it for next cycle
  command_received = "";

  calculateEular();
  delay(200);
}
