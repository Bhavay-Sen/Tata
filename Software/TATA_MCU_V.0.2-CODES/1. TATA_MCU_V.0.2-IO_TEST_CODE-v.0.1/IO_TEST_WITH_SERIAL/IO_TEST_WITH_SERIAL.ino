
void setup() {
  // put your setup code here, to run once:
  
  // UART - Enable
  Serial.begin(9600);

  //LED Pins
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  // DIP Switch pins
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("##################### I/O TEST CODE WITH SERIAL OUT##############################");

  digitalWrite(3, HIGH);
  delay(100);
  digitalWrite(3,LOW);
  delay(100);
  
  digitalWrite(4, HIGH);
  delay(100);
  digitalWrite(4,LOW);
  delay(100);

  for(int i = 0 ; i < 4 ; i++){
    Serial.print("PIN "); Serial.print(i+5); Serial.print(" "); Serial.println(digitalRead(i+5));
  }
  Serial.println();

  // Analog read sequence 


  delay(2000);
}
