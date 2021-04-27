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
float stgtmp1 = 40;      //Stage 1 default temperature
float stgtmp2 = 50;      //Stage 2 default temperature
float stgtmp3 = 60;      //Stage 3 default temperature
float stgtmp4 = 60;      //Stage 4 default temperature
float Celsius = 0;       //Declaring Celsius for Temperature Sensor
long stgtme1 = 5;        //Stage 1 Time default setting 5 minutes converted to Millis()
long stgtme2 = 60;       //Stage 2 Time default setting 1 Hours converted to Millis()
long stgtme3 = 360;      //Stage 3 Time default setting 6 Hours converted to Millis()
long stgtme4 = 5;        //Stage 4 Time default setting 5 minutes converted to MIllis()
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
bool moistCon_flagInit = true;    //Bool flag for intial weight
bool endState = true;             //Bool flag for while loop end state trap

void setup()
{
    pinMode(Buzzer, OUTPUT);       //Setting Buzzer pin mode to output
    pinMode(InfraredBulb, OUTPUT); //Setting Infrared Bulb pin mode to output
    pinMode(startB, INPUT);        //setting start button pin mode to input
    pinMode(logicBE, INPUT);       //setting logic button state E pin mode to input
    pinMode(logicBF, INPUT);       //setting logic button state F pin mode to input
    LoadCell.begin();              //Initializing load cell
    LoadCell.start(2000);          //Giving load cell to computer every 2000 milliseconds
    LoadCell.setCalFactor(436.0);  // Callibration value of the used load cell bar
    lcd.init();                    //LCD initialization
    lcd.backlight();               //LCD backlight initialization
    sensors.begin();               //Initializing Temperature Sensor
    Serial.begin(9600);            //Setting baud rate to 9600
}

void loop()
{
    machineState();
}

void machineState()
{
    while (intro_flag)
    {
        int bStateS = digitalRead(startB);
        int bStateE = digitalRead(logicBE);
        int bStateF = digitalRead(logicBF);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Mod Settings?");
        lcd.setCursor(0, 1);
        lcd.print("SELC for Yes");
        delay(250);
        lcd.setCursor(0, 1);
        lcd.print("Start for No");
        delay(250);
        if (bStateS == 1)
        {
            intro_flag = false;
        }
        if (bStateE == 1 && bStateF == 0)
        {
            intro_flag = false;
            modifyStageTimeTemp1();
        }
    }
}
//To Edit Stage Time
void editStageTime(bool stgtme_flag, int stageNumber, int stageIncDec, int stgtme, int maxTime, int minTime)
{
    while (stgtme_flag)
    {
        int bStateS = digitalRead(startB);
        int bStateE = digitalRead(logicBE);
        int bStateF = digitalRead(logicBF);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(String("Stage") + String(stageNumber) + String("Time:"));
        lcd.setCursor(0, 1);
        lcd.print(stgtme);
        delay(200);
        //Increment Stage Time
        if (bStateE == 1 && bStateF == 1)
        {
            if (stgtme <= maxTime)
            {
                stgtme = stgtme + stageIncDec;
            }
            if (stgtme > maxTime)
            {
                lcd.setCursor(0, 1);
                lcd.print("Max Time");
                delay(500);
            }
        }
        //Decrement Stage Time
        if (bStateE == 0 && bStateF == 1)
        {
            if (stgtme >= minTime)
            {
                stgtme = stgtme - stageIncDec;
            }
            if (stgtme < minTime)
            {
                lcd.setCursor(0, 1);
                lcd.print("Min Time");
                delay(500);
            }
        }
        if (bStateS == 1)
        {
            stgtme = stgtme * (60000);
            digitalWrite(Buzzer, HIGH);
            delay(100);
            digitalWrite(Buzzer, LOW);
            stgtme_flag = false;
        }
    }
    return;
}
//To Edit Stage Temperuture
void editStageTemp(bool stgtmp_flag, int stageNumber, int stageIncDec, int stgtmp, int maxTemp, int minTemp)
{
    while (stgtmp_flag)
    {
        int bStateS = digitalRead(startB);
        int bStateE = digitalRead(logicBE);
        int bStateF = digitalRead(logicBF);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(String("Stage") + String("stageNumber") + String("Temp:"));
        lcd.setCursor(0, 1);
        lcd.print(stgtmp);
        delay(200);
        //Increment Stage Temperature
        if (bStateE == 1 && bStateF == 1)
        {
            if (stgtmp <= maxTemp)
            {
                stgtmp = stgtmp + stageIncDec;
            }
            if (stgtmp > maxTemp)
            {
                lcd.setCursor(0, 1);
                lcd.print("Max Temp");
                delay(500);
            }
        }
        //Decrement Stage Temperature Sensor
        if (bStateE == 0 && bStateF == 1)
        {
            if (stgtmp >= maxTemp)
            {
                stgtmp = stgtmp - stageIncDec;
            }
            if (stgtmp < maxTemp)
            {
                lcd.setCursor(0, 1);
                lcd.print("Max Temp");
                delay(500);
            }
        }
        if (bStateS == 1)
        {
            digitalWrite(Buzzer, HIGH);
            delay(100);
            digitalWrite(Buzzer, LOW);
            stgtmp_flag = false;
        }
    }
    return;
}

void modifyStageTimeTemp1()
{
    int stagenum = 1;
    int incDec = 1;
    int maxTme = 10;
    int minTme = 3;
    int maxTmp = 60;
    int minTmp = 35;
    editStageTime(stgtme1_flag, stagenum, incDec, stgtme1, maxTme, minTme);
    editStageTemp(stgtmp1_flag, stagenum, incDec, stgtmp1, maxTmp, minTmp);
    modifyStageTimeTemp2();
}

void modifyStageTimeTemp2()
{
    int stagenum = 2;
    int incDec = 1;
    int maxTme = 90;
    int minTme = 45;
    int maxTmp = 60;
    int minTmp = 40;
    editStageTime(stgtme2_flag, stagenum, incDec, stgtme2, maxTme, minTme);
    editStageTemp(stgtmp2_flag, stagenum, incDec, stgtmp2, maxTmp, maxTme);
    modifyStageTimeTemp3();
}

void modifyStageTimeTemp3()
{
    int stagenum = 3;
    int incDec = 1;
    int maxTme = 420;
    int minTme = 45;
    int maxTmp = 60;
    int minTmp = 45;
    editStageTime(stgtme3_flag, stagenum, incDec, stgtme3, maxTme, minTme);
    editStageTemp(stgtmp3_flag, stagenum, incDec, stgtmp3, maxTmp, minTmp);
}

void editMoistureContent()
{
    while (moistCon_flag)
    {
        int bStateS = digitalRead(startB);
        int bStateE = digitalRead(logicBE);
        int bStateF = digitalRead(logicBF);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Moisture Content:");
        lcd.setCursor(0, 1);
        lcd.print(String(MC) + String("%"));
        if (bStateE == 1 && bStateF == 1)
        {
            //Increment
            if (MC <= 30)
            {
                MC = MC + 1;
            }
            if (MC > 30)
            {
                lcd.setCursor(0, 1);
                lcd.print("Max Content");
                delay(500);
            }
        }
        if (bStateE == 0 && bStateF == 1)
        {
            //Decrement
            if (MC >= 15)
            {
                MC = MC - 1;
            }
            if (MC < 15)
            {
                lcd.setCursor(0, 1);
                lcd.print("Min Content");
                delay(500);
            }
        }
        if (bStateS == 1)
        {
            digitalWrite(Buzzer, HIGH);
            delay(100);
            digitalWrite(Buzzer, LOW);
            moistCon_flag = false;
            measureInitialWeight();
        }
    }
}

void systemStateSTG1() {}

void measureInitialWeight()
{
    while (moistCon_flagInit)
    {
        int bStateS = digitalRead(startB);
        LoadCell.update();
        float iWeight = LoadCell.getData();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("InitialWeight:");
        lcd.setCursor(0, 1);
        lcd.print(String(iWeight) + String("Grams"));
        if (bStateS == 1)
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Initial W:");
            lcd.setCursor(0, 1);
            lcd.print(String(iWeight + String("Grams")));
            delay(500);
            digitalWrite(Buzzer, HIGH);
            delay(100);
            digitalWrite(Buzzer, LOW);
            moistCon_flagInit = false;
            systemStateSTG1();
        }
    }
}

void infraredBulbHeating() {}
