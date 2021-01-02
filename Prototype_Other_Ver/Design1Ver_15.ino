#include <LiquidCrystal_I2C.h>                            //Library for LCD
#include <Wire.h>                                         //Wire Library for LCD
#include <OneWire.h>                                      //Wire Library for Temp Sensor
#include <DallasTemperature.h>                            //Library for Temperature Sensor
#define ONE_WIRE_BUS 5                                    //temperature sensor pin
#define relay 2                                           //relay pin for heater
int startB = 11;                                          //start button pin
int logicbE = 12;                                         //button logic E 
int logicbF = 13;                                         //button logic F
int bState;                                               //State of the button
int sysState = 1;                                         //Machine System State (In LOOP)
OneWire oneWire(ONE_WIRE_BUS);                            //Temperature Wire BUS
DallasTemperature sensors(&oneWire);                      //Assign Temperature Library to sensors
LiquidCrystal_I2C lcd(0x27, 16, 2);                       //Assign LCD library to lcd
float MaxTemp1 = 40;                                      //Stage 1 Temperature
float MaxTemp2 = 50;                                      //Stage 2 Temperature
float MaxTemp3 = 60;                                      //Stage 3 Temperature
float Celcius = 0;                                        //Declaring Celcius for Temperature Sensor
unsigned long previousMillis = 0;                         //For Timing Millis() funciton
long timeInter1 = 300000;                                 //Stage 1 Time default setting 5 mins in Millis
long timeInter2 = 3600000;                                //Stage 2 Time default setting 1 Hour in Millis
long timeInter3 = 21600000;                               //Stage 3 Time default setting 6 Hours in Millis
long timeInter4 = 300000;                                 //Stage 4 time default setting 5 mins in Millis
int stageTme1 = 5;                                        //Stage 1 Time for Modifying the setting
int stageTme2 = 60;                                       //Stage 2 Time for Modifying the setting
int stageTme3 = 360;                                      //Stage 3 Time for Modifying the setting
int stageTme4 = 5;                                        //Stage 4 Time for Modifying the setting

void setup() {
  pinMode(relay, OUTPUT);                                 //setting relay mode to output
  pinMode(logicbE, INPUT);                                //setting logic E to input
  pinMode(logicbF, INPUT);                                //setting logic F to input
  pinMode(startB, INPUT);                                 //setting start to input
  lcd.init();                                             //acquiring lcd
  lcd.backlight();                                        //turning on backlight LCD
  sensors.begin();                                        //Temperature Sensor
  Serial.begin(9600);                                     //Serial 9600 
}
void loop() {
  sysState1();
}

void sysState1(){
  int bStateS = digitalRead(startB);
  int bStateE = digitalRead(logicbE);
  int bStateF = digitalRead(logicbF);

  if(bStateS == 0 && bStateE == 0 && bStateF == 0){
    //Do nothing State Trap
    bState = 0;
  }
  else if(bStateS == 1){
    //Start button pressed
    bState = 1;
  }
  else if(bStateE == 1 && bStateF == 0){
    //Select button pressed
    bState = 2;
  }
  buttonStateVer(bState);
}

void buttonStateVer(int bStateVer){
  switch(bStateVer){
    case 0:
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Mod Settings?");
      lcd.setCursor(0,1);
      lcd.print("SEL for Yes");
      delay(2000);
      lcd.setCursor(0,1);
      lcd.print("Start for No");
      delay(2000);
      //Do nothing case
      break;
    }
    case 1:
    {
      //go to function to start stage 1
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Starting");
      lcd.setCursor(0,1);
      lcd.print("Stage 1");
      delay(1000);
      Serial.println("START HEATING");
      while(millis() - previousMillis <= timeInter1){
        heatingSTG1();
      }
      digitalWrite(relay, LOW);
      //adding bstate here
      break;
    }
    case 2:
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Mod Settings");
      delay(2000);
      modStage1();
    }
  }
}
void sysStartSTG1(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("START");
  delay(2000);
  heatingSTG1();
}
void modStage1(){
  int bStateS = digitalRead(startB);
  int bStateE = digitalRead(logicbE);
  int bStateF = digitalRead(logicbF);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Time Stage 1:");
  lcd.setCursor(0,1);
  lcd.print(stageTme1);
  delay(500);
  if(bStateS == 0){
    //going back to function if Start is not pressed
    modStage1();
  }
  else if(bState == 1){
    //go to next function
  }
  else if(bStateE == 1 && bStateF == 1){
    //up button for increment time
    if(stageTme1 <= 10){
     stageTme1 = stageTme1 + 1; 
    }
    else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Max Time Reached ");
      delay(1000);
    }
    modStage1();
  }
  else if(bStateE == 0 && bStateF == 1){
    //down button for decrement time
    if(stageTme1 >= 3){
     stageTme1 = stageTme1 - 1; 
    }
    else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Min Time Reached");
      delay(1000);
    }
    modStage1();
  }
}
void modStage2(){
  
}
void heatingSTG1(){
  sensors.requestTemperatures();
  Celcius = sensors.getTempCByIndex(0);
  Serial.print("C");
  Serial.println(Celcius);
  lcd.setCursor(0,0);
  lcd.print("TEMP: ");
  lcd.print(Celcius);
  if(Celcius >= MaxTemp1){
    digitalWrite(relay, LOW);
    lcd.setCursor(0,1);
    lcd.print("Max Temp");
  }
  else{
    digitalWrite(relay, HIGH);
    lcd.setCursor(0,1);
    lcd.print("Heating");
  }
  delay(500);
}
