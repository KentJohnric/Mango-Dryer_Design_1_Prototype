#include <HX711_ADC.h>
#include <Wire.h>
HX711_ADC LoadCell(9, 10);

void setup() {
  LoadCell.begin();
  LoadCell.start(2000);
  LoadCell.setCalFactor(445);
  Serial.begin(9600);
}

void loop() {
  LoadCell.update();
  float i = LoadCell.getData();
  Serial.print("Weight[g]:");
  Serial.println(i);
}
