int val = 0;             // Used for storing key values
int button = 26;         // Connect the pin of the button to IO26

void setup() {
  Serial.begin(115200);      
  pinMode(button, INPUT);    
}

void loop() {
  val = digitalRead(button);  // Read the value of the key
  Serial.print(val);          // print
  if (val == 0) {             // When the button is pressed, a low level is read and the relevant information is printed
    Serial.println("\t Press the botton");
    delay(100);
  }

  else {                      // print release information of the button
    Serial.println("\t Loosen the botton");
    delay(100);
  }
}