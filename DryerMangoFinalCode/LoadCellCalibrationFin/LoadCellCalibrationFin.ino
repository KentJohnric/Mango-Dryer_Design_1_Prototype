#include <LiquidCrystal.h>
#include <HX711_ADC.h>
#include <Wire.h>
HX711_ADC LoadCell(9, 10);
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //Setting up the variables to pins number
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool moistCon_flagInit = true;

void setup() {
  lcd.begin(16,2);
  LoadCell.begin();
  LoadCell.start(2000);
  LoadCell.setCalFactor(445);
  Serial.begin(9600);
}

void loop() {
  lcd.clear();
  while(moistCon_flagInit){
    LoadCell.update();
    float iWeight = LoadCell.getData();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(iWeight);
    Serial.print("Weight[g]:");
    Serial.println(iWeight);
  }
}
