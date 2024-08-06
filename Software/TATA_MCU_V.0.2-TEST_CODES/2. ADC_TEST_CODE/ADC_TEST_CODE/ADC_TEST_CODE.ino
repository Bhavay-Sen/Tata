void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  float Vref = 4.32;
  unsigned int adcPinA0 = analogRead(A0);
  unsigned int adcPinA1 = analogRead(A1);
  unsigned int adcPinA2 = analogRead(A2);

Serial.println();
  Serial.println("############################");

  Serial.println(adcPinA0);
  Serial.print(" ADC A0 : "); Serial.println( (adcPinA0 * Vref) / 1024.0 );
  Serial.println(adcPinA1);
  Serial.print(" ADC A1 : "); Serial.println( (adcPinA1 * Vref) / 1024.0 );
  Serial.println(adcPinA2);
  Serial.print(" ADC A2 : "); Serial.println( (adcPinA2 * Vref) / 1024.0 );

Serial .println();
  delay(2000);
}
