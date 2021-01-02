#include <LiquidCrystal_I2C.h>        //Library for LCD
#include <Wire.h>                     //Wire Library for LCD
#include <OneWire.h>                  //Wire Library for Temp Sensor
#include <DallasTemperature.h>        //Library for Temperature Sensor
#define ONE_WIRE_BUS 5                //temperature sensor pin
#define relay 2                       //relay pin
#define startB 9                  //start button pin
#define logicbE 10                    //button logic E 
#define logicbF 11                    //button logic F
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
  //pinMode(startB, INPUT);
  lcd.init();
  lcd.backlight();
  sensors.begin();
  Serial.begin(9600);
}

void loop() {
  
  int stateb = digitalRead(startB);
  int stateE = digitalRead(logicbE);
  int stateF = digitalRead(logicbF);
  Serial.print("Start Button");
  Serial.println(stateb);
  Serial.print("E");
  Serial.println(stateE);
  Serial.print("F");
  Serial.println(stateF);
  delay(1000);
}
