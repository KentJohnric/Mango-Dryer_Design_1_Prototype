#include <LiquidCrystal_I2C.h>                            //Library for LCD
#include <Wire.h>                                         //Wire Library for LCD
#include <OneWire.h>                                      //Wire Library for Temp Sensor
#include <DallasTemperature.h>                            //Library for Temperature Sensor
#include <HX711_ADC.h>									  //Library for Load Sensor IC HX711
#define ONE_WIRE_BUS 5                                    //temperature sensor pin
#define heater 2                                          //relay pin for heater
#define BuzzerP 7										  //Buzzer PIN
#define MotorR 8										  //Motor relay PIN
int startB = 11;                                          //start button pin
int logicbE = 12;                                         //button logic E 
int logicbF = 13;                                         //button logic F
int bState;                                               //State of the button
int sysState = 1;                                         //Machine System State (In LOOP)
HX711_ADC LoadCell(4, 6);								  //4 is Data Out and 6 is Clock for Load Sensor
OneWire oneWire(ONE_WIRE_BUS);                            //Temperature Wire BUS
DallasTemperature sensors(&oneWire);                      //Assign Temperature Library to sensors
LiquidCrystal_I2C lcd(0x27, 16, 2);                       //Assign LCD library to lcd
float MaxTemp1 = 40;                                      //Stage 1 Temperature
int editedTemp1 = 40;									  //Modded Stage 1 Temperature
float MaxTemp2 = 50;                                      //Stage 2 Temperature
int editedTemp2 = 50;									  //Modded Stage 2 Temperature
float MaxTemp3 = 60;                                      //Stage 3 Temperature
int editedTemp3 = 60;									  //Modded Stage 3 Temperature
float Celcius = 0;                                        //Declaring Celcius for Temperature Sensor
int moistureConVal = 25;								  //Declaring Moisture content default value
float editMoistureConVal = 25;							  //Moddded Moisture content value
unsigned long previousMillis = 0;                         //For Timing Millis() funciton
long timeInter1 = 300000;                                 //Stage 1 Time default setting 5 mins in Millis
long timeInter2 = 3600000;                                //Stage 2 Time default setting 1 Hour in Millis
long timeInter3 = 21600000;                               //Stage 3 Time default setting 6 Hours in Millis
long timeInter4 = 300000;                                 //Stage 4 time default setting 5 mins in Millis
int editedTme1 = 5;										  //Modded Stage 1 Time
int editedTme2 = 60;									  //Modded Stage 2 Time
int editedTme3 = 360;									  //Modded Stage 3 Time
const long interWeight = 150000;						  //Giving Weight sensor time to compute weight
const long interWss = 60000;							  //Giving machine 1 min to display moisture content
float initialW;											  //variable for Initial Weight
float finalW;											  //variable for Final Weight
float mContent;											  //variable for Moisture Content
bool intro_flag = true;									  //Bool flag for while loop init
bool stgtme1_flag = true;								  //Bool flag for while loop stage 1 time
bool stgtmp1_flag = true;								  //Bool flah for while loop stage 1 temperature
bool stgtme2_flag = true;								  //Bool flag for while loop stage 2 time
bool stgtmp2_flag = true;								  //Bool flag for while loop stage 2 temperature
bool stgtme3_flag = true;								  //Bool flag for while loop stage 3 time
bool stgtmp3_flag = true;								  //Bool flag for while loop stage 3 temperature
bool moistCon_flag = true;								  //Bool flag for while loop Moisture Content
bool endStateM = true;									  //Bool flag for while loop end process

void setup() {
	pinMode(BuzzerP, OUTPUT);							//Setting buzzer pin to OUTPUT
	pinMode(MotorR, OUTPUT);							//Setting relay Motor pin to OUTPUT
	pinMode(heater, OUTPUT);							//setting relay heater pin to OUTPUT
	pinMode(logicbE, INPUT);							//Setting logic E pin mode to INPUT
	pinMode(logicbF, INPUT);							//Setting logic F pin mode to INPUT
	pinMode(startB, INPUT);								//Setting start button pin to INPUT
	LoadCell.begin();									//Initializing load cell
	LoadCell.start(2000);								//Giving Load Cell to compute every 2000 milliseconds
	LoadCell.setCalFactor(436.0);		 				//Callibration value of the used Load Bar
	lcd.init();											//LCD Initialization
	lcd.backlight();									//Turning LCD backlight on
	sensors.begin();									//Initializing temperature sensor 
	Serial.begin(9600);									//Serial baud rate 9600
}

void loop() {
	sysButState();
}

void sysButState() {
	while (intro_flag) {
		int bStateS = digitalRead(startB);
		int bStateE = digitalRead(logicbE);
		int bStateF = digitalRead(logicbF);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Mod Settings?");
		lcd.setCursor(0, 1);
		lcd.print("SEL for Yes");
		delay(500);
		lcd.setCursor(0, 1);
		lcd.print("Start for No");
		delay(500);
		if (bStateS == 1) {
			//start heating process
			intro_flag = false;
			sysStateSTG1();
			delay(250);
		}
		if (bStateE == 1 && bStateF == 0) {
			//go to modify settings
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("Test Buzzer");
			lcd.setCursor(0, 1);
			lcd.print("Test Motor");
			digitalWrite(BuzzerP, HIGH);
			digitalWrite(MotorR, HIGH);
			delay(1000);
			digitalWrite(BuzzerP, LOW);
			digitalWrite(BuzzerP, LOW);
			intro_flag = false;
			modStageTme1();
			delay(250);
		}
	}
}

void modStageTme1() {
	while (stgtme1_flag) {
		int bStateS = digitalRead(startB);
		int bStateE = digitalRead(logicbE);
		int bStateF = digitalRead(logicbF);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Stage 1 Time: ");
		lcd.setCursor(0, 1);
		lcd.print(editedTme1);
		delay(250);
		if (bStateE == 1 && bStateF == 1) {
			//up button for increment
			if (editedTme1 < 11) {
				editedTme1 = editedTme1 + 1;
				delay(100);
			}
			else {
				lcd.setCursor(0, 1);
				lcd.print("Max Time");
				delay(500);
			}
		}
		if (bStateE == 0 && bStateF == 1) {
			if (editedTme1 > 2) {
				editedTme1 = editedTme1 - 1;
				delay(100);
			}
			else {
				lcd.setCursor(0, 1);
				lcd.print("Min Time");
				delay(500);
			}
		}
		if (bStateS == 1) {
			editedTme1 = editedTme1 * (60000);
			timeInter1 = editedTme1;
			digitalWrite(BuzzerP, HIGH);
			delay(250);
			digitalWrite(BuzzerP, LOW);
			stgtme1_flag = false;
			modStageTmp1();
			delay(250);
		}
	}
}

void modStageTmp1() {
	while (stgtmp1_flag) {
		int bStateS = digitalRead(startB);
		int bStateE = digitalRead(logicbE);
		int bStateF = digitalRead(logicbF);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Stage 1 Temp: ");
		lcd.setCursor(0, 1);
		lcd.print(editedTemp1);
		delay(250);
		if (bStateE == 1 && bStateF == 1) {
			//up button for increment
			if (editedTemp1 < 61) {
				editedTemp1 = editedTemp1 + 5;
				delay(100);
			}
			else {
				lcd.setCursor(0, 1);
				lcd.print("Max Temp");
				delay(500);
			}
		}
		if (bStateE == 0 && bStateF == 1) {
			if (editedTemp1 > 34) {
				editedTemp1 = editedTemp1 - 5;
				delay(100);
			}
			else {
				lcd.setCursor(0, 1);
				lcd.print("Min Temp");
				delay(500);
			}
		}
		if (bStateS == 1) {
			MaxTemp1 = editedTemp1;
			digitalWrite(BuzzerP, HIGH);
			delay(250);
			digitalWrite(BuzzerP, LOW);
			stgtmp1_flag = false;
			modStageTme2();
			delay(250);
		}
	}
}

void modStageTme2() {
	while (stgtme2_flag) {
		int bStateS = digitalRead(startB);
		int bStateE = digitalRead(logicbE);
		int bStateF = digitalRead(logicbF);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Stage 2 Time: ");
		lcd.setCursor(0, 1);
		lcd.print(editedTme2);
		delay(250);
		if (bStateE == 1 && bStateF == 1) {
			//up button for increment
			if (editedTme2 < 91) {
				editedTme2 = editedTme2 + 2;
				delay(100);
			}
			else {
				lcd.setCursor(0, 1);
				lcd.print("Max Time");
				delay(500);
			}
		}
		if (bStateE == 0 && bStateF == 1) {
			if (editedTme2 > 45) {
				editedTme2 = editedTme2 - 2;
				delay(100);
			}
			else {
				lcd.setCursor(0, 1);
				lcd.print("Min Time");
				delay(500);
			}
		}
		if (bStateS == 1) {
			editedTme2 = editedTme2 * (60000);
			timeInter2 = editedTme2;
			digitalWrite(BuzzerP, HIGH);
			delay(250);
			digitalWrite(BuzzerP, LOW);
			stgtme2_flag = false;
			modStageTmp2();
			delay(250);
		}
	}
}

void modStageTmp2() {
	while (stgtmp2_flag) {
		int bStateS = digitalRead(startB);
		int bStateE = digitalRead(logicbE);
		int bStateF = digitalRead(logicbF);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Stage 2 Temp: ");
		lcd.setCursor(0, 1);
		lcd.print(editedTemp2);
		delay(250);
		if (bStateE == 1 && bStateF == 1) {
			//up button for increment
			if (editedTemp2 < 61) {
				editedTemp2 = editedTemp2 + 5;
				delay(100);
			}
			else {
				lcd.setCursor(0, 1);
				lcd.print("Max Temp");
				delay(500);
			}
		}
		if (bStateE == 0 && bStateF == 1) {
			if (editedTemp2 > 39) {
				editedTemp2 = editedTemp2 - 5;
				delay(100);
			}
			else {
				lcd.setCursor(0, 1);
				lcd.print("Min Temp");
				delay(500);
			}
		}
		if (bStateS == 1) {
			MaxTemp2 = editedTemp2;
			digitalWrite(BuzzerP, HIGH);
			delay(250);
			digitalWrite(BuzzerP, LOW);
			stgtmp2_flag = false;
			modStageTme3();
			delay(250);
		}
	}
}

void modStageTme3() {
	while (stgtme3_flag) {
		int bStateS = digitalRead(startB);
		int bStateE = digitalRead(logicbE);
		int bStateF = digitalRead(logicbF);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Stage 3 Time: ");
		lcd.setCursor(0, 1);
		lcd.print(editedTme3);
		delay(250);
		if (bStateE == 1 && bStateF == 1) {
			//up button for increment
			if (editedTme3 < 481) {
				editedTme3 = editedTme3 + 20;
				delay(100);
			}
			else {
				lcd.setCursor(0, 1);
				lcd.print("Max Time");
				delay(500);
			}
		}
		if (bStateE == 0 && bStateF == 1) {
			if (editedTme3 > 300) {
				editedTme3 = editedTme3 - 20;
				delay(100);
			}
			else {
				lcd.setCursor(0, 1);
				lcd.print("Min Time");
				delay(500);
			}
		}
		if (bStateS == 1) {
			editedTme3 = editedTme3 * (60000);
			timeInter3 = editedTme3;
			digitalWrite(BuzzerP, HIGH);
			delay(250);
			digitalWrite(BuzzerP, LOW);
			stgtme3_flag = false;
			modStageTmp3();
			delay(250);
		}
	}
}

void modStageTmp3() {
	while (stgtmp3_flag) {
		int bStateS = digitalRead(startB);
		int bStateE = digitalRead(logicbE);
		int bStateF = digitalRead(logicbF);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Stage 3 Temp: ");
		lcd.setCursor(0, 1);
		lcd.print(editedTemp3);
		delay(250);
		if (bStateE == 1 && bStateF == 1) {
			//up button for increment
			if (editedTemp3 < 61) {
				editedTemp3 = editedTemp3 + 5;
				delay(100);
			}
			else {
				lcd.setCursor(0, 1);
				lcd.print("Max Temp");
				delay(500);
			}
		}
		if (bStateE == 0 && bStateF == 1) {
			if (editedTemp3 > 44) {
				editedTemp3 = editedTemp3 - 5;
				delay(100);
			}
			else {
				lcd.setCursor(0, 1);
				lcd.print("Min Temp");
				delay(500);
			}
		}
		if (bStateS == 1) {
			MaxTemp3 = editedTemp3;
			digitalWrite(BuzzerP, HIGH);
			delay(250);
			digitalWrite(BuzzerP, LOW);
			stgtmp3_flag = false;
			modMoistCon();
			delay(250);
		}
	}
}

void modMoistCon() {
	while (moistCon_flag) {
		int bStateS = digitalRead(startB);
		int bStateE = digitalRead(logicbE);
		int bStateF = digitalRead(logicbF);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Moisture Con: ");
		lcd.setCursor(0, 1);
		lcd.print(editMoistureConVal);
		if (bStateE == 1 && bStateF == 1) {
			//increment
			if (editMoistureConVal < 25) {
				editMoistureConVal = editMoistureConVal + 1;
				delay(100);
			}
			else {
				lcd.setCursor(0, 1);
				lcd.print("Max Content");
				delay(500);
			}
		}
		if (bStateE == 0 && bStateF == 1) {
			//decrement
			if (editMoistureConVal > 15) {
				editMoistureConVal = editMoistureConVal - 1;
				delay(100);
			}
			else {
				lcd.setCursor(0, 1);
				lcd.print("Min Content");
				delay(500);
			}
		}
		if (bStateS == 1) {
			//Start next stage
			moistureConVal = editMoistureConVal;
			digitalWrite(BuzzerP, HIGH);
			delay(250);
			digitalWrite(BuzzerP, LOW);
			moistCon_flag = false;
			initialWeight();
			delay(250);
		}
	}
}

void initialWeight() {
	previousMillis = millis();
	while (millis() - previousMillis <= interWeight) {
		int bStateS = digitalRead(startB);
		LoadCell.update();
		float i = LoadCell.getData();
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("InitialWeight: ");
		lcd.setCursor(0, 1);
		lcd.print(i);
		lcd.print("[Grams]");
		initialW = i;
		delay(250);
		if (bStateS == 1) {
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("Done InitW");
			lcd.setCursor(0, 1);
			lcd.print("[Grams]");
			lcd.print(initialW);
			delay(1000);
			digitalWrite(BuzzerP, HIGH);
			delay(250);
			digitalWrite(BuzzerP, LOW);
			sysStateSTG1();
			delay(250);
		}
	}
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Done InitW");
	lcd.setCursor(0, 1);
	lcd.print("[GRAMS]");
	lcd.print(initialW);
	delay(1000);
	digitalWrite(BuzzerP, HIGH);
	delay(250);
	digitalWrite(BuzzerP, LOW);
	sysStateSTG1();
	delay(250);
}

void sysStateSTG1() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("STARTING");
	lcd.setCursor(0, 1);
	lcd.print("Stage 1");
	delay(1000);
	digitalWrite(BuzzerP, HIGH);
	delay(250);
	digitalWrite(BuzzerP, LOW);
	previousMillis = millis();
	digitalWrite(MotorR, HIGH);
	while (millis() - previousMillis <= timeInter1) {
		unsigned long timerLCDraw = millis() - previousMillis;
		unsigned long timerLCD = timerLCDraw / 60000;
		int bStateS = digitalRead(startB);
		sensors.requestTemperatures();
		Celcius = sensors.getTempCByIndex(0);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("TEMP: ");
		lcd.print(Celcius);
		lcd.setCursor(0, 1);
		lcd.print("Time: ");
		lcd.print(timerLCD);
		if (Celcius >= MaxTemp1) {
			digitalWrite(heater, LOW);
			lcd.setCursor(0, 1);
			lcd.print("Max Temp");
		}
		else {
			digitalWrite(heater, HIGH);
		}
		if (bStateS == 1) {
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("To Stage 2");
			delay(2000);
			digitalWrite(heater, LOW);
			digitalWrite(MotorR, LOW);
			sysStateSTG2();
		}
		delay(500);
	}
	digitalWrite(heater, LOW);
	digitalWrite(MotorR, LOW);
	sysStateSTG2();
}

void sysStateSTG2() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Starting");
	lcd.setCursor(0, 1);
	lcd.print("Stage 2");
	delay(1000);
	digitalWrite(BuzzerP, HIGH);
	delay(250);
	digitalWrite(BuzzerP, LOW);
	digitalWrite(MotorR, HIGH);
	previousMillis = millis();
	while (millis() - previousMillis <= timeInter2) {
		unsigned long timerLCDraw = millis() - previousMillis;
		unsigned long timerLCD = timerLCDraw / 60000;
		int bStateS = digitalRead(startB);
		sensors.requestTemperatures();
		Celcius = sensors.getTempCByIndex(0);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("TEMP: ");
		lcd.print(Celcius);
		lcd.setCursor(0, 1);
		lcd.print("Timer: ");
		lcd.print(timerLCD);
		if (Celcius >= MaxTemp2) {
			digitalWrite(heater, LOW);
			lcd.setCursor(0, 1);
			lcd.print("Max Temp");
		}
		else {
			digitalWrite(heater, HIGH);
		}
		if (bStateS == 1) {
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("To Stage 3");
			delay(2000);
			digitalWrite(heater, LOW);
			digitalWrite(MotorR, LOW);
			sysStateSTG3();
			delay(250);
		}
		delay(500);
	}
	digitalWrite(heater, LOW);
	digitalWrite(MotorR, LOW);
	sysStateSTG3();
}

void sysStateSTG3() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Starting");
	lcd.setCursor(0, 1);
	lcd.print("Stage 3");
	delay(1000);
	digitalWrite(BuzzerP, HIGH);
	delay(250);
	digitalWrite(BuzzerP, LOW);
	digitalWrite(MotorR, HIGH);
	previousMillis = millis();
	while (millis() - previousMillis <= timeInter3) {
		unsigned long timerLCDraw = millis() - previousMillis;
		unsigned long timerLCD = timerLCDraw / 60000;
		int bStateS = digitalRead(startB);
		sensors.requestTemperatures();
		Celcius = sensors.getTempCByIndex(0);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("TEMP: ");
		lcd.print(Celcius);
		lcd.setCursor(0, 1);
		lcd.print("Timer: ");
		lcd.print(timerLCD);
		if (Celcius >= MaxTemp2) {
			digitalWrite(heater, LOW);
			lcd.setCursor(0, 1);
			lcd.print("Max Temp");
		}
		else {
			digitalWrite(heater, HIGH);
		}
		if (bStateS == 1) {
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("To Stage 4");
			delay(2000);
			digitalWrite(heater, LOW);
			digitalWrite(MotorR, LOW);
			sysStageSTG4();
			delay(250);
		}
		delay(500);
	}
	digitalWrite(heater, LOW);
	digitalWrite(MotorR, LOW);
	sysStageSTG4();
}

void sysStageSTG4() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Calculating");
	lcd.setCursor(0, 1);
	lcd.print("Moisture Con");
	delay(500);
	digitalWrite(BuzzerP, HIGH);
	delay(250);
	digitalWrite(BuzzerP, LOW);
	previousMillis = millis();
	while (millis() - previousMillis <= interWeight) {
		LoadCell.update();
		float i = LoadCell.getData();
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("FinalWeight");
		lcd.setCursor(0, 1);
		lcd.print(i);
		lcd.print("[Grams]");
		finalW = i;
		delay(250);
	}
	delay(500);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Computing");
	delay(500);
	digitalWrite(BuzzerP, HIGH);
	delay(250);
	digitalWrite(BuzzerP, LOW);
	previousMillis = millis();
	mContent = ((initialW - finalW) / initialW) * 100;
	while (millis() - previousMillis <= interWss) {
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("MoistureContent");
		lcd.setCursor(0, 1);
		lcd.print(mContent);
		lcd.print("%");
		delay(500);
	}
	if (mContent <= moistureConVal) {
		//end process
		endState();
		delay(250);
	}
	else {
		//Heat again
		digitalWrite(BuzzerP, HIGH);
		delay(250);
		digitalWrite(BuzzerP, LOW);
		heatAg();
		delay(250);
	}
}

void endState() {
	while (endStateM){
		int bStateS = digitalRead(startB);
		int bStateE = digitalRead(logicbE);
		int bStateF = digitalRead(logicbF);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("MContent");
		lcd.setCursor(0, 1);
		lcd.print("Reached");
		delay(250);
		if (bStateS == 1) {
			endStateM = false;
			sysButState();
			delay(100);
		}
		delay(500);
	}
}

void heatAg() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Heating Again");
	lcd.setCursor(0, 1);
	lcd.print("Continue");
	delay(1000);
	digitalWrite(BuzzerP, HIGH);
	delay(250);
	digitalWrite(BuzzerP, LOW);
	digitalWrite(MotorR, HIGH);
	previousMillis = millis();
	while (millis() - previousMillis <= timeInter1) {
		int bStateS = digitalRead(startB);
		unsigned long timerLCDraw = millis() - previousMillis;
		unsigned long timerLCD = timerLCDraw / 60000;
		sensors.requestTemperatures();
		Celcius = sensors.getTempCByIndex(0);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print(Celcius);
		lcd.setCursor(0, 1);
		lcd.print("Timer: ");
		lcd.print(timerLCD);
		if (Celcius >= MaxTemp3) {
			digitalWrite(heater, LOW);
			lcd.setCursor(0, 1);
			lcd.print("Max Temp");
		}
		else {
			digitalWrite(heater, HIGH);
		}
		if (bStateS == 1) {
			digitalWrite(heater, LOW);
			digitalWrite(MotorR, LOW);
			sysStageSTG4();
			delay(500);
		}
		delay(500);
	}
	digitalWrite(heater, LOW);
	digitalWrite(MotorR, LOW);
	sysStageSTG4();
	delay(500);
}