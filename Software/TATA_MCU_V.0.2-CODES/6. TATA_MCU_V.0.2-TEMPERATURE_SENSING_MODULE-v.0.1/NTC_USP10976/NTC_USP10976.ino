#include <USP10976.h>

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
}
void loop() {
  float temp = temperatureSense(5.0, 1, A0, 1.0);
  Serial.print("temperature : ");
  Serial.println(temp, 4);
  delay(1000);
}