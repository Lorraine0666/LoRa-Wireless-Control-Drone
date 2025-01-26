#include "imu.h"
#include "port.h"
#include <MPU6500_WE.h>
#include <Wire.h>
#define MPU6500_ADDR 0x68

MPU6500_WE myMPU6500 = MPU6500_WE(MPU6500_ADDR);

// calculate offset
// 计算偏移量
// a variable to keep record the time of loops
// 计算偏移量时的循环次数
float i;

// parameters
// 参数
// radian to angle conversion factor
// 弧度到角度的换算系数
float rad2deg = 57.29578;
// angular speed on x-axis and y-axis
// 换算到x, y轴上的角速度
float roll_v = 0, pitch_v = 0;

// define differential time
// 定义微分时间
float now = 0, lasttime = 0, dt = 0;

// three state
// 三个状态
// priori state, observed state, optimal estimation state
// 先验状态，观测状态，最优估计状态
// angle calculated by gyroscope integration, which is priori state
// 陀螺仪积分计算出的角度，先验状态
float gyro_roll = 0, gyro_pitch = 0;
// angle observed by accelerometer, which is observation status
// 加速度计观测出的角度，观测状态
float acc_roll = 0, acc_pitch = 0;
float k_roll = 0, k_pitch = 0; // 卡尔曼滤波后估计出最优角度，最优估计状态

float roll = 0, pitch = 0;

/*误差协方差矩阵P*/
float e_P[2][2] = {
    {1, 0},
    {0, 1}}; // 误差协方差矩阵，这里的e_P既是先验估计的P，也是最后更新的P

/*卡尔曼增益K*/
float k_k[2][2] = {{0, 0}, {0, 0}}; // 这里的卡尔曼增益矩阵K是一个2X2的方阵

void imu_setup() {
  Wire.setSCL(I2C_SCL);
  Wire.setSDA(I2C_SDA);
  Wire.begin();
  if (!myMPU6500.init()) {
    Serial2.println("MPU6500 does not respond");
  } else {
    Serial2.println("MPU6500 is connected");
  }
  Serial2.println(
      "Position you MPU6500 flat and don't move it - calibrating...");
  delay(1000);
  myMPU6500.autoOffsets();
  Serial2.println("Done!");

  myMPU6500.enableGyrDLPF();
  myMPU6500.setGyrDLPF(MPU6500_DLPF_6);
  myMPU6500.setSampleRateDivider(5);
  myMPU6500.setGyrRange(MPU6500_GYRO_RANGE_250);
  myMPU6500.setAccRange(MPU6500_ACC_RANGE_2G);
  myMPU6500.enableAccDLPF(true);
  myMPU6500.setAccDLPF(MPU6500_DLPF_6);
}

void calculateEular() {
  // 获得x y z轴的加速度数据
  xyzFloat gValue = myMPU6500.getGValues();
  // 获得x y z轴的角速度数据
  xyzFloat gyr = myMPU6500.getGyrValues();
  float temp = myMPU6500.getTemperature();
  float resultantG = myMPU6500.getResultantG(gValue);

  /*计算微分时间*/
  now = millis();                 // 当前时间(ms)
  dt = (now - lasttime) / 1000.0; // 微分时间(s)
  lasttime = now;                 // 上一次采样时间(ms)

  /*step1:利用陀螺仪获得角速度对欧拉角的估计值 */
  roll_v =
      gyr.x + ((sin(k_pitch) * sin(k_roll)) / cos(k_pitch)) * gyr.y +
      ((sin(k_pitch) * cos(k_roll)) / cos(k_pitch)) * gyr.z; // roll轴的角速度
  pitch_v = cos(k_roll) * gyr.y - sin(k_roll) * gyr.z;       // pitch轴的角速度
  gyro_roll = k_roll + dt * roll_v;                          // 先验roll角度
  gyro_pitch = k_pitch + dt * pitch_v;                       // 先验pitch角度

  /*step2:计算先验误差协方差矩阵P*/ // 这里的Q矩阵是一个对角阵且对角元均为0.0025
  e_P[0][0] = e_P[0][0] + 0.0025;
  e_P[0][1] = e_P[0][1] + 0;
  e_P[1][0] = e_P[1][0] + 0;
  e_P[1][1] = e_P[1][1] + 0.0025;

  /*step3:更新卡尔曼增益K*/
  k_k[0][0] = e_P[0][0] / (e_P[0][0] + 0.3);
  k_k[0][1] = 0;
  k_k[1][0] = 0;
  k_k[1][1] = e_P[1][1] / (e_P[1][1] + 0.3);

  /*step4:计算最优估计状态*/
  /*观测状态*/

  // 利用加速度计 计算roll角度
  acc_roll = atan(gValue.y / gValue.z) * rad2deg;
  // 利用加速度计 pitch角度
  acc_pitch = -1 * atan(gValue.x / sqrt(sq(gValue.y) + sq(gValue.z))) * rad2deg;
  /*最优估计状态*/
  k_roll = gyro_roll + k_k[0][0] * (acc_roll - gyro_roll);
  k_pitch = gyro_pitch + k_k[1][1] * (acc_pitch - gyro_pitch);

  roll = 3.0 * k_roll;
  pitch = 3.0 * k_pitch;
}