#include <Arduino.h>           //arduino library for platform.io
#include <LiquidCrystal_I2C.h> //Library for LCD
#include <Wire.h>              //Wire Library for LCD
#include <OneWire.h>           //Wire library for Temperature Sensor DS18B20
#include <DallasTemperature.h> //Temperature Sensor Library for DS18B20
#include <HX711_ADC.h>         //Library for Load Cell IC HX711

#define ONE_WIRE_BUS A2 //Temperature Sensor (DS18B20) A2 pin
#define InfraredBulb 5  //Infrared Emitter Bulb 5 pin
#define Buzzer 6        //Buzzer pin 6

int startB = 6;  //Start Button Pin 6
int logicBE = 4; //Button pin 4 Logic State E
int logicBF = 5; //Button pin 5 logic State F

HX711_ADC LoadCell(A0, 13);          //DataOUT A0 pin and 13 SCK pin
OneWire oneWire(ONE_WIRE_BUS);       //Temperature Wire BUS
DallasTemperature sensors(&oneWire); //Assign Temperature Library to sensors
LiquidCrystal_I2C lcd(0x27, 16, 2);  //Assign LCD library to LCD

int moistureConVal = 25; //Delcaring Moisture Content Default value
float stgtpm1 = 40;      //Stage 1 default temperature
float stgtmp2 = 50;      //Stage 2 default temperature
float stgtmp3 = 60;      //Stage 3 default temperature
float stgtmp4 = 60;      //Stage 4 default temperature
float Celsius = 0;       //Declaring Celsius for Temperature Sensor
long stgtme1 = 300000;   //Stage 1 Time default setting 5 minutes converted to Millis()
long stgtme2 = 3600000;  //Stage 2 Time default setting 1 Hours converted to Millis()
long stgtme3 = 21600000; //Stage 3 Time default setting 6 Hours converted to Millis()
long stgtme4 = 300000;   //Stage 4 Time default setting 5 minutes converted to MIllis()
float fWeight;           //Final Weight Declaration
float iWeight;           //Initial Weight Declaration
float MC;                // Moisture Content Declaration
float MCC;               //Computed Moisture Content Declaration

unsigned long previousMillis = 0; //For Timing Millis() function
bool intro_flag = true;           //Bool flag for while loop init trap
bool stgtme1_flag = true;         //Bool flag for while loop stage 1 time edit trap
bool stgtmp1_flag = true;         //Bool flag for while loop stage 1 temperature edit trap
bool stgtme2_flag = true;         //Bool flag for while loop stage 2 time edit trap
bool stgtmp2_flag = true;         //Bool flag for while loop stage 2 temperature edit trap
bool stgtme3_flag = true;         //Bool flag for while loop stage 3 time edit trap
bool stgtmp3_flag = true;         //Bool flag for while loop stage 3 temperature edit trap
bool moistCon_flag = true;        //Bool flag for while loop Moisture Content edit trap
bool endState = true;             //Bool flag for while loop end state trap

void setup()
{
}

void loop()
{
}