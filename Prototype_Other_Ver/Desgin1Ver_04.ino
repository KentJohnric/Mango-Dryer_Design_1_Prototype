#include <LiquidCrystal_I2C.h>        //Library for LCD
#include <Wire.h>                     //Wire Library for LCD
#include <OneWire.h>                  //Wire Library for Temp Sensor
#include <DallasTemperature.h>        //Library for Temperature Sensor
#define ONE_WIRE_BUS 5                //temperature sensor pin
#define relay 2                       //relay pin
int startB = 11;                     //start button pin
int logicbE = 12;                    //button logic E 
int logicbF = 13;                    //button logic F
OneWire oneWire(ONE_WIRE_BUS);        //Temperature Wire BUS
DallasTemperature sensors(&oneWire);  //Assign Temperature Library to sensors
LiquidCrystal_I2C lcd(0x27, 16, 2);   //Assign LCD library to lcd
float MaxTemp = 60;
float Celcius = 0;


void setup() {
  pinMode(relay, OUTPUT);
  pinMode(logicbE, INPUT);
  pinMode(logicbF, INPUT);
  pinMode(startB, INPUT);
  lcd.init();
  lcd.backlight();
  sensors.begin();
  Serial.begin(9600);
}

void loop() {
  int bstateE = digitalRead(logicbE);
  int bstateF = digitalRead(logicbF);
  int bstateS = digitalRead(startB);

  Serial.print("Logic E: ");
  Serial.println(bstateE);
  Serial.print("Logic F: ");
  Serial.println(bstateF);
  Serial.print("Start: ");
  Serial.println(bstateS);
  lcd.setCursor(0,0);
  lcd.print(bstateE);
  lcd.print(bstateF);
  lcd.print(bstateS);
  delay(500);
}
