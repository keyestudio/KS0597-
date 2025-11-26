const int INA = 5;
const int INB = 13;
const int buttonPin = 26; // button pin

int currentDirection = 1; // 1=forward 2=reverse
bool motorRunning = false;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;

void setup() {
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  int buttonReading = digitalRead(buttonPin);
  
  // Press the button (down edge)
  if (buttonReading == LOW && millis() - lastDebounceTime > debounceDelay) {
    lastDebounceTime = millis();
    
    if (!motorRunning) {
      if (currentDirection == 1) {
        digitalWrite(INA, HIGH); // forward
        digitalWrite(INB, LOW);
      } else {
        digitalWrite(INA, LOW); // reverse
        digitalWrite(INB, HIGH);
      }
      
      motorRunning = true;
      delay(1000); // delay 1s
      
      digitalWrite(INA, LOW);
      digitalWrite(INB, LOW);
      motorRunning = false;
      
      // change direction
      currentDirection = (currentDirection == 1) ? 2 : 1; 
    }
  }
}