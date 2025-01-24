#include <Arduino.h>
#include <port.h>
#include <MPU6500_WE.h>
#include <Wire.h>
#include <imu.h>
#include <test.h>

extern float roll;
extern float pitch;
int cur_state = 0;

void setup() {
  test_setup();
  imu_setup();
}

void loop() {
  // 检查串口是否有数据可用
  if (Serial1.available() > 0) {
    // 读取一个字节的数据
    char receivedChar = Serial1.read();
    // 打印接收到的数据
    Serial1.print("Received: ");
    Serial1.println(receivedChar);
    // 可以根据接收到的字符执行相应的操作
    // 例如，如果接收到 'A'，执行动作 1
    if (receivedChar == 'A') {
      cur_state = 1;
    }
    else if(receivedChar == 'S')
      cur_state = 0;
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
  
  calculateEular();
  Serial1.print("Eular Angle x, y:");
  Serial1.print(roll);
  Serial1.print(",");
  Serial1.println(pitch);
  Serial1.print("State Now:");
  Serial1.print(cur_state);
  delay(100);

}