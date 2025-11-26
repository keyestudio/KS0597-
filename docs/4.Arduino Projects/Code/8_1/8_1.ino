/*  
 * Name: 8_1
 * Function: PIR sensor detects human movement
 * IDE：ARDUINO 2.3.4
 * Auther: https://www.keyestudio.com/
 */
int val = 0;
int PIRPin = 25;   //Define the pin of the PIR sensor as GPIO25
void setup() {
  Serial.begin(115200);   //set baud rate to 115200
  pinMode(PIRPin, INPUT);    //Set the sensor to input mode
}

void loop() {
  val = digitalRead(PIRPin);    //Read the sensor value
  Serial.print(val);    //Print the sensor value
  if (val == 1) {    //If there is someone moving nearby, output a high level
    Serial.println("\t Some body is in this area!");
    delay(100);
  }
  else {    //If no one is moving nearby, output a low level
    Serial.println("\t No one!");
    delay(100);
  }
}