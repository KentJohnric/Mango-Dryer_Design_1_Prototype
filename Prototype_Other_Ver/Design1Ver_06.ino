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
int bState;
unsigned long previousMillis = 0;
const long timeInter = 60000;

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
  int bStateS = digitalRead(startB);
  int bStateE = digitalRead(logicbE);
  int bStateF = digitalRead(logicbF);

  if(bStateS == 0 && bStateE == 0 && bStateF == 0){
    bState = 0;
  }
  else if(bStateE == 1 && bStateF == 0){
    bState = 1;
  }
  else if(bStateE == 1 && bStateF == 1){
    bState = 2;
  }
  else if(bStateE == 0 && bStateF == 1){
    bState = 3;
  }
  else if(bStateS == 1){
    bState = 4;
  }
  buttonStateVer(bState);
}

void buttonStateVer(int bStateVer){
   switch(bStateVer){
    case 0:
    {
      //Do nothing
      break;
    }
    case 1:
    {
      Serial.println("Select");
      break;
    }
    case 2:
    {
      Serial.println("UP");
      break;
    }
    case 3:
    {
      Serial.println("DOWN");
      break;
    }
    case 4:
    {
      Serial.println("START");
      Serial.print("HEATING");
      while(millis() - previousMillis <= timeInter){
        heating();
      }
      break;
    }
  }
  return;
}

void heating(){
  sensors.requestTemperatures();
  Celcius = sensors.getTempCByIndex(0);
  Serial.print("C");
  Serial.println(Celcius);
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(Celcius);
  if(Celcius >= MaxTemp){
    digitalWrite(relay, LOW);
    lcd.setCursor(0,1);
    lcd.print("Max temp");
  }
  else{
    digitalWrite(relay, HIGH);
    lcd.setCursor(0,1);
    lcd.print("Heating");
  }
  delay(500);
}
