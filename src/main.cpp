#include <Arduino.h>
#include <port.h>
#include <MPU6500_WE.h>
#include <Wire.h>
#include <imu.h>
#include <SoftwareSerial.h>  // 引入软件串口库

extern float roll;
extern float pitch;

// 定义 HC-14 的 TX 和 RX 引脚
#define HC14_TX 10  // HC-14 的 TX 引脚连接到 Arduino 的数字引脚 10
#define HC14_RX 11  // HC-14 的 RX 引脚连接到 Arduino 的数字引脚 11

// 创建软件串口实例
SoftwareSerial hc14(HC14_RX, HC14_TX);

int cur_state = 0;

void setup() {
  // 初始化串口
  Serial2.begin(115200);  // 用于调试
  hc14.begin(115200);       // 

  // 初始化 IMU 模块
  imu_setup();

  // 初始化 PWM 引脚
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);

  Serial2.println("HC-14 初始化完成");
}

void loop() {
  // 检查 HC-14 是否有数据可用
  if (hc14.available()) {
    char receivedChar = hc14.read();  // 读取 HC-14 接收到的数据
    Serial2.print("接收到指令: ");
    Serial2.println(receivedChar);

    // 根据接收到的指令执行操作
    if (receivedChar == 'A') {
      cur_state = 1;  // 执行动作
    } else if (receivedChar == 'S') {
      cur_state = 0;  // 停止动作
    }

    // 发送确认消息
    hc14.println("received");
    Serial2.println("发送确认消息: received");
  }

  // 根据 cur_state 执行相应的 PWM 控制
  if (cur_state == 1) {
    analogWrite(PWM1, 10);  // 示例 PWM 值
    analogWrite(PWM2, 10);
    analogWrite(PWM3, 10);
    analogWrite(PWM4, 10);
  } else {
    analogWrite(PWM1, 0);  // 停止 PWM 输出
    analogWrite(PWM2, 0);
    analogWrite(PWM3, 0);
    analogWrite(PWM4, 0);
  }

  // 计算欧拉角并打印
  calculateEular();
  Serial2.print("欧拉角 x, y: ");
  Serial2.print(roll);
  Serial2.print(", ");
  Serial2.println(pitch);
  Serial2.print("当前状态: ");
  Serial2.println(cur_state);

  delay(100);
}