#include <LiquidCrystal_I2C.h>        //Library for LCD
#include <Wire.h>                     //Wire Library for LCD
#include <OneWire.h>                  //Wire Library for Temp Sensor
#include <DallasTemperature.h>        //Library for Temperature Sensor
#define ONE_WIRE_BUS 9                //temperature sensor pin
#define relay 2                       //relay pin
#define startB 11                     //start button pin
#define logicbE 12                    //button logic E 
#define logicbF 13                    //button logic F
OneWire oneWire(ONE_WIRE_BUS);        //Temperature Wire BUS
DallasTemperature sensors(&oneWire);  //Assign Temperature Library to sensors
LiquidCrystal_I2C lcd(0x27, 16, 2);   //Assign LCD library to lcd
float MaxTemp = 60;
float Celcius = 0;
int bstateE = 0;
int bstateF = 0;
int bstateS = 0;

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
  while(digitalRead(logicbE) == LOW && digitalRead(logicbF) == LOW){
    //Do Nothing
  }
  if(digitalRead(startB) == HIGH){
    heating();
  }
  else if(digitalRead(logicbE) == LOW && digitalRead(logicbF == HIGH)){
    //Down Button
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Pressed DOWN");
  }
  else if(digitalRead(logicbE) == HIGH && digitalRead(logicbF == HIGH)){
    //Up Button
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Pressed UP");
  }
  else if(digitalRead(logicbE) == HIGH && digitalRead(logicbF == LOW)){
    //SEL Button
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Pressed DOWN");
  }
  delay(1000);
}
void heating(){
  sensors.requestTemperatures();
  Celcius = sensors.getTempCByIndex(0);
  Serial.println("C");
  Serial.print(Celcius);
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(Celcius);
  if(Celcius >= MaxTemp){
    digitalWrite(relay, HIGH);
    lcd.setCursor(0,1);
    lcd.print("Max Temp");
  }
  else{
    digitalWrite(relay, LOW);
    lcd.setCursor(0,1);
    lcd.print("HEATING!");
  }
  delay(500);
}
