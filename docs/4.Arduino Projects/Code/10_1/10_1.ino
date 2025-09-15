#include <Stepper.h>             // provide the basic functions for controlling stepper motor

// motor parameters（28BYJ-48）
const int STEPS_PER_REV = 2038;  // Actual steps/turn (may vary by motor)
const int MOTOR_PIN1 = 14;       // IN1
const int MOTOR_PIN2 = 27;       // IN2
const int MOTOR_PIN3 = 16;       // IN3
const int MOTOR_PIN4 = 17;       // IN4

// User-adjustable parameters
int motorSpeed = 10;      // Rotational speed(RPM). It is recommended to set it at 5-12. If it exceeds 15, it is very likely to get stuck.
int rotationCount = 2;    // the number of rotation turns
bool reverseDirection = false; // Reverse flag position

// Initialize the stepper motor (note the pin sequence: IN1-IN3-IN2-IN4)
Stepper myStepper(STEPS_PER_REV, MOTOR_PIN1, MOTOR_PIN3, MOTOR_PIN2, MOTOR_PIN4);

void setup() {
  Serial.begin(115200);
}

void loop() {
  // reversal test
  Serial.println("Open curtain");
  rotateMotor(rotationCount, true);
  delay(1000);

  // forward test
  Serial.println("Close curtain");
  rotateMotor(rotationCount, false);
  delay(1000);  // pause for 1s
}

// Motor rotation function
void rotateMotor(int turns, bool reverse) {
  myStepper.setSpeed(motorSpeed);
  int steps = STEPS_PER_REV * turns * (reverse ? -1 : 1);
  myStepper.step(steps);
}