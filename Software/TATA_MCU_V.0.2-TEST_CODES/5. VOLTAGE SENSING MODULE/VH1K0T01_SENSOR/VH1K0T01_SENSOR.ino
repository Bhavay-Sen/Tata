#include <VH1K0T01.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  float Voltage = terminalVoltage(5.0, 1, A0, 39.8, 174.9);
  Serial.print("Voltage : "); Serial.print(Voltage); Serial.println();
  delay(500);
}