#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init(); // initialize the lcd
  lcd.backlight();
  Serial.begin(9600);
  lcd.setCursor(0, 0);
  lcd.print("Hello World!");
  lcd.setCursor(0, 1);
  lcd.print("Arduino I2C LCD");
}

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print("Hello World!");
  delay(500);
}