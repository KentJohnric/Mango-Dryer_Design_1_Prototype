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

  if(bstateS == 0 && bstateE == 0 && bstateF == 0){
    lcd.setCursor(0,0);
    lcd.print("edit settings?");
    lcd.setCursor(0,1);
    lcd.print("SEL to Edit");
  }
  else if(bstateS == 1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Initializing");
    lcd.setCursor(0,1);
    lcd.print("Stage 1");
    heating();
    delay(5000);
  }
  else if(bstateE == 1 && bstateF == 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Editing");
    lcd.setCursor(0,1);
    lcd.print("Stage 1");
  }
}
void heating(){
  lcd.clear();
  sensors.requestTemperatures();
  Celcius = sensors.getTempCByIndex(0);
  Serial.println("C");
  Serial.print(Celcius);
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(Celcius);
  int timer = delay(500);
  for()
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
