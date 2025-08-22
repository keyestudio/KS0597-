#include <Stepper.h>             // 提供了控制步进电机的基本功能

// 电机参数（28BYJ-48）
const int STEPS_PER_REV = 2038;  // 实际步数/圈（不同电机可能有差异）
const int MOTOR_PIN1 = 14;       // IN1
const int MOTOR_PIN2 = 27;       // IN2
const int MOTOR_PIN3 = 16;       // IN3
const int MOTOR_PIN4 = 17;       // IN4

// 用户可调参数
int motorSpeed = 10;      // 转速(RPM)，建议5-12，超过15极易堵转
int rotationCount = 2;    // 转动圈数
bool reverseDirection = false; // 反转标志位

// 初始化步进电机（注意引脚顺序IN1-IN3-IN2-IN4）
Stepper myStepper(STEPS_PER_REV, MOTOR_PIN1, MOTOR_PIN3, MOTOR_PIN2, MOTOR_PIN4);

void setup() {
  Serial.begin(115200);
}

void loop() {
  // 反转测试
  Serial.println("拉开窗帘");
  rotateMotor(rotationCount, true);
  delay(1000);

  // 正转测试
  Serial.println("关闭窗帘");
  rotateMotor(rotationCount, false);
  delay(1000);  // 停顿1秒
}

// 电机转动函数
void rotateMotor(int turns, bool reverse) {
  myStepper.setSpeed(motorSpeed);
  int steps = STEPS_PER_REV * turns * (reverse ? -1 : 1);
  myStepper.step(steps);
}