#include <Arduino.h>

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int value = analogRead(A0);
  Serial.print(value);
  delay(500);
}