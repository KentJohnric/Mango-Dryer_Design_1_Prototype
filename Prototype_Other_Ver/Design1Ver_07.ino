#include <LiquidCrystal_I2C.h>                            //Library for LCD
#include <Wire.h>                                         //Wire Library for LCD
#include <OneWire.h>                                      //Wire Library for Temp Sensor
#include <DallasTemperature.h>                            //Library for Temperature Sensor
#define ONE_WIRE_BUS 5                                    //temperature sensor pin
#define relay 2                                           //relay pin
int startB = 11;                                          //start button pin
int logicbE = 12;                                         //button logic E 
int logicbF = 13;                                         //button logic F
int bState;                                               //Starting Machine State
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
int bStateS = digitalRead(startB);
int bStateE = digitalRead(logicbE);
int bStateF = digitalRead(logicbF);

void setup() {
  pinMode(relay, OUTPUT);                                 //setting relay mode to output
  pinMode(logicbE, INPUT);                                //setting logic E to input
  pinMode(logicbF, INPUT);                                //setting logic F to input
  pinMode(startB, INPUT);                                 //setting start to input
  lcd.init();                                             //acquiring lcd
  lcd.backlight();                                        //turning on backlight LCD
  sensors.begin();                                        //Temperature Sensor
  Serial.begin(9600);                                     //Serial 9600 
  lcd.setCursor(0,0);                                     //Setting lcd cursor to top
  lcd.print("Welcome :)");                                //printing to lcd Welcome
}
void loop() {
  bState = 0;                                             //Setting System State to 0
  
  sysState(bState);                                       //System State Function Call
}

void sysState(int bStateVer){
  switch(bStateVer){
    case 0: //Do nothing State
    {
      //Do nothing
      break;
    }
    case 1: // prompting user to mod settings
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("UP for Yes");
      lcd.setCursor(0,1);
      lcd.print("DOWN for No");
      while(bStateE == 0 && bStateF == 0){
        //Do nothing
      }
      if(bStateE == 1 && bStateF == 1){
        //UP button pressed
        bState = 2;
      }
      else if(bStateE == 0 && bStateF == 1){
        //Down button pressed
      }
    }
    case 2: //Modding the Settings starts here
    {
      
    }
  }
}

void modprompt(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("UP for Yes");
  lcd.setCursor(0,1);
  lcd.print("DOWN for No");
  if(bStateE == 1 && bStateF == 1){
    bState = 1; //State for MOD
  }
  else if(bStateE == 0 && bStateF == 1){
    bState = 2; //Start State
  }
  while(bStateE == 0 && bStateF == 0){
    //Do nothing
  }
}
