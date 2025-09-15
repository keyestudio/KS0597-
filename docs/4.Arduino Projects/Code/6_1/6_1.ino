int val = 0;
void setup() {
  Serial.begin(115200);   
  pinMode(18, INPUT);    
}

void loop() {
  val = digitalRead(18);  //Read the digital level
  Serial.print(val);     //Print the read level signal
  if (val == 0) {  //Obstacle detection
    Serial.println("\t There are obstacles");
    delay(100);
  }
  else {  //No obstacles were found.
    Serial.println("\t All going well");
    delay(100);
  }
}