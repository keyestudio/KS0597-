#include <Stepper.h>

// 电机参数（28BYJ-48）
const int STEPS_PER_REV = 2038;  // 实际步数/圈
const int MOTOR_PIN1 = 14;       // IN1
const int MOTOR_PIN2 = 27;       // IN2 
const int MOTOR_PIN3 = 16;       // IN3
const int MOTOR_PIN4 = 17;       // IN4

// 光敏传感器
const int LIGHT_SENSOR_PIN = 34; // 光敏模块接GPIO34
const int LIGHT_THRESHOLD = 2000; // 光照阈值（根据实际调整）

// 电机控制参数
const int MOTOR_SPEED = 10;      // 转速(RPM)
bool isCurtainOpen = true;       // 默认窗帘初始状态为打开
const int TURNS = 2;             // 转动圈数设为2圈

Stepper myStepper(STEPS_PER_REV, MOTOR_PIN1, MOTOR_PIN3, MOTOR_PIN2, MOTOR_PIN4);

void setup() {
  Serial.begin(115200);
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  myStepper.setSpeed(MOTOR_SPEED);
  closeCurtain(); // 设置窗帘状态为关闭
  delay(5000);    // 安装等待时间
}

void loop() {
  int lightValue = analogRead(LIGHT_SENSOR_PIN);
  Serial.print("光照值: ");
  Serial.println(lightValue);

  if(lightValue < LIGHT_THRESHOLD && !isCurtainOpen) {
    // 光线不足且窗帘关闭时，打开窗帘
    openCurtain();
  } 
  else if(lightValue >= LIGHT_THRESHOLD && isCurtainOpen) {
    // 光线充足且窗帘打开时，关闭窗帘
    closeCurtain();
  }
  
  delay(2000); // 每2秒检测一次
}

void openCurtain() {
  myStepper.step(-STEPS_PER_REV * TURNS); // 反转2圈
  isCurtainOpen = true;
}

void closeCurtain() {
  myStepper.step(STEPS_PER_REV * TURNS); // 正转2圈
  isCurtainOpen = false;
}