#include "ACS712_20.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A1, INPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  float seriesCurrent = currentSense(4.66, 10, A1);
  // seriesCurrent = (seriesCurrent > 0) ? (seriesCurrent = seriesCurrent) : seriesCurrent = 0;
  Serial.print("Current : "); Serial.print(seriesCurrent+0.30); Serial.println(" A");
  delay(1000);
}