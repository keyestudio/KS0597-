// Define pins
const int INA = 5;  
const int INB = 13; 

void setup() {
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  // Adjust the waiting time for the flag
  digitalWrite(INA, LOW);
  digitalWrite(INB, LOW);
  delay(8000);
}

void loop() {
  // forward rotate 1s
  digitalWrite(INA, HIGH);
  digitalWrite(INB, LOW);
  delay(1000);

  // stop 2s
  digitalWrite(INA, LOW);
  digitalWrite(INB, LOW);
  delay(2000);

  // reverse rotate 1s
  digitalWrite(INA, LOW);
  digitalWrite(INB, HIGH);
  delay(1000);

  // stop 2s
  digitalWrite(INA, LOW);
  digitalWrite(INB, LOW);
  delay(2000);
}