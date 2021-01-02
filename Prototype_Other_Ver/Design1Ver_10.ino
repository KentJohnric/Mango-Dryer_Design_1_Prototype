#include <LiquidCrystal_I2C.h>                            //Library for LCD
#include <Wire.h>                                         //Wire Library for LCD
#include <OneWire.h>                                      //Wire Library for Temp Sensor
#include <DallasTemperature.h>                            //Library for Temperature Sensor
#define ONE_WIRE_BUS 5                                    //temperature sensor pin
#define relay 2                                           //relay pin
int startB = 11;                                          //start button pin
int logicbE = 12;                                         //button logic E 
int logicbF = 13;                                         //button logic F
int sysState = 1;                                             //Machine System State (In LOOP)
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
  int bStateS = digitalRead(startB);
  int bStateE = digitalRead(logicbE);
  int bStateF = digitalRead(logicbF);
  
  if(bStateE == 1 && bStateF == 1 && sysState == 0){
    //Up button pressed Modify settings
  }
  else if(bStateS == 1){
    //Start button pressed start default settings 
  }
  sysStateF(sysState);
  delay(1000);
}
void sysStateF(int sysStateFVer){
  switch(sysStateFVer){
    case 1: //Initializing Machine Stage prompting user to modify
    {
      int bStateS = digitalRead(startB);
      int bStateE = digitalRead(logicbE);
      int bStateF = digitalRead(logicbF);
      while(bStateE == 0 && bStateF == 0 && bStateS == 0){
      Serial.println("Modify Settings");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Modify");
      lcd.setCursor(0,1);
      lcd.print("Settings?");
      delay(500);
      }
      if(bStateE == 1 && bStateF == 1 && bStateS == 0){
        break;
      }
      else if(bStateE == 0 && bStateF == 0 && bStateS == 1){
        sysStateFVer++;
        if(bStateS == 1){
          break;
        }
      }
    }
    case 2: //Mod User State Stage 1
    {
      incStage1TME();
      break;
    }
  }
}
void incStage1TME(){
    int bStateS = digitalRead(startB);
    int bStateE = digitalRead(logicbE);
    int bStateF = digitalRead(logicbF);
    Serial.println("Stage 1 Modifying Setting");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Stage 1 Mod");
    lcd.setCursor(0,1);
    lcd.print("Time: ");
    lcd.print(stageTme1);

  if(bStateE == 1 && bStateF == 1){
    if(stageTme1 <= 10){
      stageTme1 = stageTme1 +1;
    }
    else{
      lcd.setCursor(0,1);
      lcd.print("Max Time");
    }
  }
  else if(bStateE == 0 && bStateF == 1){
    if(stageTme1 >= 3){
      stageTme1 = stageTme1 - 1;
    }
    else{
      lcd.setCursor(0,1);
      lcd.print("Min Time");
    }
  }
  else if(bStateS == 1){
    sysState++;
  }
  return;
}
