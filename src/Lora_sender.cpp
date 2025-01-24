// 文件名：lora_sender.cpp
#include <SPI.h>
#include <LoRa.h>

// LoRa模块的引脚定义
#define SS_PIN 10
#define RST_PIN 9
#define DI0_PIN 2

void setup() {
  Serial.begin(115200); // 初始化串口
  while (!Serial) delay(10); // 等待串口连接

  // 初始化LoRa模块
  SPI.begin(SS_PIN);
  LoRa.setPins(SS_PIN, RST_PIN, DI0_PIN);
  if (!LoRa.begin(433E6)) { // 设置LoRa频率为433MHz
    Serial.println("LoRa初始化失败");
    while (1);
  }
  Serial.println("LoRa初始化成功");
}

void loop() {
  if (Serial.available()) { // 检查串口是否有指令输入
    char command = Serial.read(); // 从串口读取指令
    if (command == 'A' || command == 'S') { // 仅处理有效指令
      LoRa.beginPacket(); // 开始发送LoRa数据包
      LoRa.write(command); // 发送指令
      LoRa.endPacket(); // 结束发送
      Serial.print("发送指令: ");
      Serial.println(command);
    }
  }
}