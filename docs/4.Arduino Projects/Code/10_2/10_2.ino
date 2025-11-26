#include <Stepper.h>

// Motor parameters（28BYJ-48）
const int STEPS_PER_REV = 2038;  // Actual steps/turn
const int MOTOR_PIN1 = 14;       // IN1
const int MOTOR_PIN2 = 27;       // IN2 
const int MOTOR_PIN3 = 16;       // IN3
const int MOTOR_PIN4 = 17;       // IN4

// Photoresistor
const int LIGHT_SENSOR_PIN = 34; // Connect the photoresistor to GPIO34
const int LIGHT_THRESHOLD = 2000; // Light threshold (adjust according to actual conditions)

// Motor control parameters
const int MOTOR_SPEED = 10;      // Rotation speed(RPM)
bool isCurtainOpen = true;       // Set the default initial state of the curtain to open
const int TURNS = 2;             // Set the turns of rotations to 2

Stepper myStepper(STEPS_PER_REV, MOTOR_PIN1, MOTOR_PIN3, MOTOR_PIN2, MOTOR_PIN4);

void setup() {
  Serial.begin(115200);
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  myStepper.setSpeed(MOTOR_SPEED);
}

void loop() {
  int lightValue = analogRead(LIGHT_SENSOR_PIN);
  Serial.print("light value: ");
  Serial.println(lightValue);

  if(lightValue < LIGHT_THRESHOLD && !isCurtainOpen) {
    // When there is insufficient light and the curtain is closed, open the curtain
    openCurtain();
  } 
  else if(lightValue >= LIGHT_THRESHOLD && isCurtainOpen) {
    // When there is sufficient light and the curtain is opened, close the curtain
    closeCurtain();
  }
  
  delay(2000); // Check every two seconds
}

void openCurtain() {
  myStepper.step(-STEPS_PER_REV * TURNS); // Reverse two turns
  isCurtainOpen = true;
}

void closeCurtain() {
  myStepper.step(STEPS_PER_REV * TURNS); // forward rotate two turns
  isCurtainOpen = false;
}