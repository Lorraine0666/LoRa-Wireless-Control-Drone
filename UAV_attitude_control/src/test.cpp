#include "test.h"
#include "port.h"
#include <Arduino.h>

HardwareSerial Serial1(RX1, TX1);
uint32_t counter = 0;

void test_setup() {
  // Initialize PWM pins
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);

  // 电池ADC
  pinMode(ADC_BATTERY, INPUT_ANALOG);

  // 初始化LED1作为led
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // 初始化串口1
  Serial1.begin(115200);

  // 初始化串口2
  Serial2.setRx(RX2);
  Serial2.setTx(TX2);
  Serial2.begin(115200);
}

void test() {
  // put your main code here, to run repeatedly:
  if (counter % 4 == 0){
    analogWrite(PWM1, 10);
    analogWrite(PWM2, 0);
    analogWrite(PWM3, 0);
    analogWrite(PWM4, 0);
  }
  else if (counter % 4 == 1){
    analogWrite(PWM1, 0);
    analogWrite(PWM2, 10);
    analogWrite(PWM3, 0);
    analogWrite(PWM4, 0);
  }
  else if (counter % 4 == 2){
    analogWrite(PWM1, 0);
    analogWrite(PWM2, 0);
    analogWrite(PWM3, 10);
    analogWrite(PWM4, 0);
  }
  else if (counter % 4 == 3){
    analogWrite(PWM1, 0);
    analogWrite(PWM2, 0);
    analogWrite(PWM3, 0);
    analogWrite(PWM4, 10);
  }
  int battery = analogRead(ADC_BATTERY);
  float valtage = battery * 11 * 3.3 / 1023;
  Serial1.print("Battery voltage: ");
  Serial1.println(valtage);
  Serial2.print("Battery voltage: ");
  Serial2.println(valtage);

  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  delay(500);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  delay(500);
  counter++;
}
