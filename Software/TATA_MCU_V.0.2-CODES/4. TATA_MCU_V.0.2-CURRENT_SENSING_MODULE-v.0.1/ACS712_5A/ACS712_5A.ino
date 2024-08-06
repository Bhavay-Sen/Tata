#include "ACS712_5.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A1, INPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  float seriesCurrent = currentSense(4.67, 10, A1);
  // seriesCurrent = (seriesCurrent > 0) ? (seriesCurrent = seriesCurrent) : seriesCurrent = 0;
  Serial.print("Current : "); Serial.print(seriesCurrent + 0.20); Serial.println(" A");
  delay(1000);
}