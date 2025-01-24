// 文件名：main.cpp
#include <Arduino.h>
#include "src/port.h"
#include <MPU9250_WE.h>
#include <Wire.h>
#include "src/imu.h"
#include <SPI.h>
#include "LoRa.h"  // 引入LoRa库

extern float roll;
extern float pitch;
int cur_state = 0;

// LoRa模块的引脚定义
#define SS_PIN 10
#define RST_PIN 9
#define DI0_PIN 2

void setup() {
  // 初始化串口
  Serial.begin(115200);

  // 初始化LoRa模块
  SPI.begin();
  LoRa.setPins(SS_PIN, RST_PIN, DI0_PIN);
  if (!LoRa.begin(433E6)) { // 设置LoRa频率为433MHz
    Serial.println("LoRa初始化失败");
    while (1);
  }
  Serial.println("LoRa初始化成功");

  // 初始化IMU模块
  imu_setup();
}

void loop() {
  // 检查LoRa是否有数据可用
  if (LoRa.parsePacket()) {
    char receivedChar = LoRa.read(); // 读取LoRa数据
    Serial.print("接收到指令: ");
    Serial.println(receivedChar);

    // 根据接收到的指令执行操作
    if (receivedChar == 'A') {
      cur_state = 1; // 执行动作1
    } else if (receivedChar == 'S') {
      cur_state = 0; // 停止动作
    }
  }

  // 根据cur_state执行相应的PWM控制
  if (cur_state == 1) {
    analogWrite(PWM1, 10); // 示例PWM值
    analogWrite(PWM2, 10);
    analogWrite(PWM3, 10);
    analogWrite(PWM4, 10);
  } else {
    analogWrite(PWM1, 0); // 停止PWM输出
    analogWrite(PWM2, 0);
    analogWrite(PWM3, 0);
    analogWrite(PWM4, 0);
  }

  // 计算欧拉角并打印
  calculateEular();
  Serial.print("欧拉角 x, y: ");
  Serial.print(roll);
  Serial.print(", ");
  Serial.println(pitch);
  Serial.print("当前状态: ");
  Serial.println(cur_state);

  delay(100);
}