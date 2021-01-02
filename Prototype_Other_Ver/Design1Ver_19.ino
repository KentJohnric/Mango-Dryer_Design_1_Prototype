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
long stageTme1 = 5;                                        //Stage 1 Time for Modifying the setting//
int stageTme2 = 60;                                       //Stage 2 Time for Modifying the setting
int stageTme3 = 360;                                      //Stage 3 Time for Modifying the setting
int stageTme4 = 5;                                        //Stage 4 Time for Modifying the setting

void setup()
{
	pinMode(relay, OUTPUT);									//setting relay pin mode to OUTPUT
	pinMode(logicbE, INPUT);								//setting logic E pin mode to INPUT
	pinMode(logicbF, INPUT);								//setting logic F pin mode to INPUT
	pinMode(startB, INPUT);									//setting start button pin mode to INPUT
	lcd.init();												//LCD initialization
	lcd.backlight();										//turning LCD backlight
	sensors.begin();										//Initilizing temperature sensor
	Serial.begin(9600);										//serial baud rate 9600
}

void loop(){
	sysButState();
}

void sysButState() {
	int bStateS = digitalRead(startB);
	int bStateE = digitalRead(logicbE);
	int bStateF = digitalRead(logicbF);

	if (bStateS == 0 && bStateE == 0 && bStateF == 0) {
		//Go to Do nothing state Switch Trap
		bState = 0;
	}
	else if (bStateS == 1) {
		//Go to Start Stage 1 State
		bState = 1;
	}
	else if (bStateE == 1 && bStateF == 0) {
		//Go to Modify State
		bState = 2;
	}
	systemState(bState);									//Passing bstate value to the argument
}

void systemState(int sysStateVer) {
	switch (sysStateVer) {
	case 0: {
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Mod Settings?");
		lcd.setCursor(0, 1);
		lcd.print("SEL for Yes");
		delay(2000);
		lcd.setCursor(0, 1);
		lcd.print("START for No");
		delay(1000);
		break;
	}
	case 1: {
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("STARTING");
		lcd.setCursor(0, 1);
		lcd.print("Stage 1");
		delay(1000);
	}
	case 2: {
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Initializing");
		lcd.setCursor(0, 1);
		lcd.print("Mod Settings");
		delay(2000);
		modStage1();
	}
	}
}

void modStage1() {
	delay(1000);
	int bStateSTG1 = 2;
	int bStateS = digitalRead(startB);
	int bStateE = digitalRead(logicbE);
	int bStateF = digitalRead(logicbF);
	if (bStateE == 1 && bStateF == 1) {
		stageTme1 = stageTme1++;
		delay(250);
	}
	else if (bStateE == 0 && bStateF == 1) {
		stageTme1 = stageTme1++;
		delay(250);
	}
	else if (bStateS == 1) {
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Editing Stage 2");
		delay(1000);
		modStage2();
	}
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Stage 1 Time: ");
	lcd.setCursor(0, 1);
	lcd.print(stageTme1);
	modStage1();
}

void modStage2() {
}
