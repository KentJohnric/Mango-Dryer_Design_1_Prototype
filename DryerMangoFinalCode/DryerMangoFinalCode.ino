//Sardina, Kent Johnric M. | Computer Engineering | Design Dryer Final Code Revision
#include <LiquidCrystal.h>     //Library for LCD
#include <Wire.h>              //Wire Library for LCD
#include <OneWire.h>           //Wire library for Temperature Sensor DS18B20
#include <DallasTemperature.h> //Temperature Sensor Library for DS18B20
#include <HX711_ADC.h>         //Library for Load Cell IC HX711

#define ONE_WIRE_BUS A2 //Temperature Sensor (DS18B20) A2 pin
#define InfraredBulb 10 //Infrared Emitter Bulb 5 pin
#define Buzzer 9        //Buzzer pin 6
#define Fan A1          //Fan Pin A3
#define ElecFan A2      //Electronics Cooling Fan pin 9

int startB = 8;  //Start Button Pin 6
int logicBE = 6; //Button pin 4 Logic State E
int logicBF = 7; //Button pin 5 logic State F

HX711_ADC LoadCell(9, 10);                                  //DataOUT A0 pin and 13 SCK pin
OneWire oneWire(ONE_WIRE_BUS);                              //Temperature Wire BUS
DallasTemperature sensors(&oneWire);                        //Assign Temperature Library to sensors
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //Setting up the variables to pins number
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                  //Setting the defined pin to LCD

float stgtmp1 = 40;                //Stage 1 default temperature
float stgtmp2 = 50;                //Stage 2 default temperature
float stgtmp3 = 60;                //Stage 3 default temperature
float stgtmp4 = 60;                //Stage 4 default temperature
long stgtme1C;                     //Stage 1 converted time
long stgtme2C;                     //Stage 2 converted time
long stgtme3C;                     //Stage 3 converted time
long stgtme4C;                     //Stage 4 converted time
float Celsius = 0;                 //Declaring Celsius for Temperature Sensor
long stgtme1 = 5;                  //Stage 1 Time default setting 5 minutes converted to Millis()
long stgtme2 = 60;                 //Stage 2 Time default setting 1 Hours converted to Millis()
long stgtme3 = 360;                //Stage 3 Time default setting 6 Hours converted to Millis()
long stgtme4 = 5;                  //Stage 4 Time default setting 5 minutes converted to MIllis()
const long interWeight = 150000;   //Giving Delay on Weight Calculation
float fWeight;                     //Final Weight Declaration
float iWeight;                     //Initial Weight Declaration
float MC = 25;                     // Moisture Content Declaration
float MCC;                         //Computed Moisture Content Declaration
unsigned long previousMillis = 0;  //For Timing Millis() funciton
unsigned long countDownMillis = 0; //For Timing Millis() function
bool intro_flag = true;            //Bool flag for while loop init trap
bool stgtme1_flag = true;          //Bool flag for while loop stage 1 time edit trap
bool stgtmp1_flag = true;          //Bool flag for while loop stage 1 temperature edit trap
bool stgtme2_flag = true;          //Bool flag for while loop stage 2 time edit trap
bool stgtmp2_flag = true;          //Bool flag for while loop stage 2 temperature edit trap
bool stgtme3_flag = true;          //Bool flag for while loop stage 3 time edit trap
bool stgtmp3_flag = true;          //Bool flag for while loop stage 3 temperature edit trap
bool moistCon_flag = true;         //Bool flag for while loop Moisture Content edit trap
bool moistCon_flagInit = true;     //Bool flag for intial weight
bool endState = true;              //Bool flag for while loop end state trap
bool stg1state = true;             //Boolean stage 1 state for initial weight
//---------------------------------------------------------------------------------------------------------------------------------------------------//
void setup()
{
  pinMode(Buzzer, OUTPUT);       //Setting Buzzer pin mode to output
  pinMode(InfraredBulb, OUTPUT); //Setting Infrared Bulb pin mode to output
  pinMode(Fan, OUTPUT);          //Setting Infrared Heater Fan pin mode to output
  pinMode(ElecFan, OUTPUT);      //Setting Electronics Cooling Fan pin mode to output
  pinMode(startB, INPUT);        //setting start button pin mode to input
  pinMode(logicBE, INPUT);       //setting logic button state E pin mode to input
  pinMode(logicBF, INPUT);       //setting logic button state F pin mode to input
  lcd.begin(16, 2);              //Initializing LCD
  LoadCell.begin();              //Initializing load cell
  LoadCell.start(2000);          //Giving load cell to computer every 2000 milliseconds
  LoadCell.setCalFactor(445);    // Callibration value of the used load cell bar
  sensors.begin();               //Initializing Temperature Sensor
  Serial.begin(9600);            //Setting baud rate to 9600
  analogWrite(ElecFan, 255);     //Initializing Electronics Cooling Fan
  buzzerBeep();                  //Beeping Buzzer
  lcd.setCursor(0, 0);           //Setting the row and column for print
  lcd.print("Welcome!");         //Greeting the User
  delay(2000);                   //Delay to display welcome
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
    Serial.println("Mod Settings? SEL for Yes & Start for No");
    lcd.setCursor(0, 0);
    lcd.print("Mod Settings?");
    if (bStateS == 1)
    {
      intro_flag = false;
      systemStateSTG1();
    }
    if (bStateE == 1 && bStateF == 0)
    {
      intro_flag = false;
      modifyStageTime1();
    }
  }
}

/*----------------------------------------------MODIFYING SETTINGS------------------------------------*/

/*Stage 1 Time Modification function*/
void modifyStageTime1()
{
  while (stgtme1_flag)
  {
    int bStateS = digitalRead(startB);
    int bStateE = digitalRead(logicBE);
    int bStateF = digitalRead(logicBF);
    Serial.println("Stage: 1 time:" + String(stgtme1));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stage 1");
    lcd.setCursor(0, 1);
    lcd.print("Time: " + String(stgtme1));
    //Increment Stage Time
    if (bStateE == 1 && bStateF == 1)
    {
      if (stgtme1 < 10)
      {
        stgtme1 = stgtme1 + 1;
        delay(250);
      }
    }
    //Decrement Stage Time
    if (bStateE == 0 && bStateF == 1)
    {
      if (stgtme1 > 3)
      {
        stgtme1 = stgtme1 - 1;
        delay(250);
      }
    }
    //Accept Stage Time
    if (bStateS == 1)
    {
      delay(250);
      stgtme1C = stgtme1 * (60000);
      Serial.println("Ctime1:" + String(stgtme1C));
      digitalWrite(Buzzer, HIGH);
      delay(100);
      digitalWrite(Buzzer, LOW);
      Serial.println(stgtme1);
      stgtme1_flag = false;
      modifyStageTemp1();
    }
  }
}

/*Stage 1 Temperature Modification function*/
void modifyStageTemp1()
{
  while (stgtmp1_flag)
  {
    int bStateS = digitalRead(startB);
    int bStateE = digitalRead(logicBE);
    int bStateF = digitalRead(logicBF);
    Serial.println("Stage: 1 Temperature:" + String(stgtmp1));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stage 1");
    lcd.setCursor(0, 1);
    lcd.print("Temp: " + String(stgtmp1));
    //Increment Stage Temperature
    if (bStateE == 1 && bStateF == 1)
    {
      if (stgtmp1 < 60)
      {
        stgtmp1 = stgtmp1 + 1;
        delay(250);
      }
    }
    //Decrement Stage Temperature
    if (bStateE == 0 && bStateF == 1)
    {
      if (stgtmp1 > 35)
      {
        stgtmp1 = stgtmp1 - 1;
        delay(250);
      }
    }
    //Accept Stage Temperature
    if (bStateS == 1)
    {
      delay(250);
      digitalWrite(Buzzer, HIGH);
      delay(100);
      digitalWrite(Buzzer, LOW);
      Serial.println(stgtmp1);
      stgtmp1_flag = false;
      modifyStageTime2();
    }
  }
}

/*Stage 2 Time Modification function*/
void modifyStageTime2()
{
  while (stgtme2_flag)
  {
    int bStateS = digitalRead(startB);
    int bStateE = digitalRead(logicBE);
    int bStateF = digitalRead(logicBF);
    Serial.println("Stage: 2 time:" + String(stgtme2));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stage 2");
    lcd.setCursor(0, 1);
    lcd.print("Time: " + String(stgtme2));
    //Increment Stage Time
    if (bStateE == 1 && bStateF == 1)
    {
      if (stgtme2 < 90)
      {
        stgtme2 = stgtme2 + 1;
        delay(250);
      }
    }
    //Decrement Stage Time
    if (bStateE == 0 && bStateF == 1)
    {
      if (stgtme2 > 45)
      {
        stgtme2 = stgtme2 - 1;
        delay(250);
      }
    }
    //Accept Stage Time
    if (bStateS == 1)
    {
      delay(250);
      stgtme2C = stgtme2 * (60000);
      Serial.println("CTime2:" + String(stgtme2C));
      digitalWrite(Buzzer, HIGH);
      delay(100);
      digitalWrite(Buzzer, LOW);
      Serial.println(stgtme2);
      stgtme1_flag = false;
      modifyStageTemp2();
    }
  }
}

/*Stage 2 Temperature Modification function*/
void modifyStageTemp2()
{
  while (stgtmp2_flag)
  {
    int bStateS = digitalRead(startB);
    int bStateE = digitalRead(logicBE);
    int bStateF = digitalRead(logicBF);
    Serial.println("Stage: 2 Temperature:" + String(stgtmp2));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stage 2");
    lcd.setCursor(0, 1);
    lcd.print("Temp: " + String(stgtmp2));
    //Increment Stage Temperature
    if (bStateE == 1 && bStateF == 1)
    {
      if (stgtmp2 < 60)
      {
        stgtmp2 = stgtmp2 + 1;
        delay(250);
      }
    }
    //Decrement Stage Temperature
    if (bStateE == 0 && bStateF == 1)
    {
      if (stgtmp2 > 40)
      {
        stgtmp2 = stgtmp2 - 1;
        delay(250);
      }
    }
    //Accept Stage Temperature
    if (bStateS == 1)
    {
      delay(250);
      digitalWrite(Buzzer, HIGH);
      delay(100);
      digitalWrite(Buzzer, LOW);
      Serial.println(stgtmp2);
      stgtmp1_flag = false;
      modifyStageTime3();
    }
  }
}

/*Stage 3 Time Modification Function*/
void modifyStageTime3()
{
  while (stgtme3_flag)
  {
    int bStateS = digitalRead(startB);
    int bStateE = digitalRead(logicBE);
    int bStateF = digitalRead(logicBF);
    Serial.println("Stage: 1 time:" + String(stgtme3));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stage 3");
    lcd.setCursor(0, 1);
    lcd.print("Time: " + String(stgtme3));
    //Increment Stage Time
    if (bStateE == 1 && bStateF == 1)
    {
      if (stgtme3 < 420)
      {
        stgtme3 = stgtme3 + 1;
        delay(250);
      }
    }
    //Decrement Stage Time
    if (bStateE == 0 && bStateF == 1)
    {
      if (stgtme3 > 45)
      {
        stgtme3 = stgtme3 - 1;
        delay(250);
      }
    }
    //Accept Stage Time
    if (bStateS == 1)
    {
      delay(250);
      stgtme3C = stgtme3 * (60000);
      stgtme4C = stgtme4 * (60000);
      Serial.println("CTime3:" + String(stgtme3C));
      digitalWrite(Buzzer, HIGH);
      delay(100);
      digitalWrite(Buzzer, LOW);
      Serial.println(stgtme3);
      stgtme1_flag = false;
      modifyStageTemp3();
    }
  }
}

/*Stage 3 Temperature Modification Function*/
void modifyStageTemp3()
{
  while (stgtmp3_flag)
  {
    int bStateS = digitalRead(startB);
    int bStateE = digitalRead(logicBE);
    int bStateF = digitalRead(logicBF);
    Serial.println("Stage: 3 Temperature:" + String(stgtmp3));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stage 3");
    lcd.setCursor(0, 1);
    lcd.print("Temp: " + String(stgtmp3));
    //Increment Stage Temperature
    if (bStateE == 1 && bStateF == 1)
    {
      if (stgtmp3 < 60)
      {
        stgtmp3 = stgtmp3 + 1;
        delay(250);
      }
    }
    //Decrement Stage Temperature
    if (bStateE == 0 && bStateF == 1)
    {
      if (stgtmp3 > 45)
      {
        stgtmp3 = stgtmp3 - 1;
        delay(250);
      }
    }
    //Accept Stage Temperature
    if (bStateS == 1)
    {
      delay(250);
      digitalWrite(Buzzer, HIGH);
      delay(100);
      digitalWrite(Buzzer, LOW);
      Serial.println(stgtmp3);
      stgtmp1_flag = false;
      modifyMoistureContent();
    }
  }
}

/*Moisture Content Modification Function*/
void modifyMoistureContent()
{
  lcd.clear();
  while (moistCon_flag)
  {
    int bStateS = digitalRead(startB);
    int bStateE = digitalRead(logicBE);
    int bStateF = digitalRead(logicBF);
    Serial.println("Moisture Content: " + String(MC));
    lcd.setCursor(0, 0);
    lcd.print("Moisture Content:");
    lcd.setCursor(0, 1);
    lcd.print(String(MC) + String("%"));
    if (bStateE == 1 && bStateF == 1)
    {
      //Increment
      if (MC < 30)
      {
        MC = MC + 1;
        delay(250);
      }
    }
    if (bStateE == 0 && bStateF == 1)
    {
      //Decrement
      if (MC > 15)
      {
        MC = MC - 1;
        delay(250);
      }
    }
    if (bStateS == 1)
    {
      delay(250);
      dispVarSet();
      digitalWrite(Buzzer, HIGH);
      delay(100);
      digitalWrite(Buzzer, LOW);
      moistCon_flag = false;
      systemStateSTG1();
    }
  }
}

/*----------------------------------------------HEATING START-----------------------------------------*/

/*Stage 1 heating process*/
void systemStateSTG1()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Stage 1 Heating");
  lcd.setCursor(0, 1);
  delay(1000);
  digitalWrite(Buzzer, HIGH);
  delay(100);
  digitalWrite(Buzzer, LOW);
  countDownMillis = millis();
  analogWrite(Fan, 255);
  while (stg1state)
  {
    int bStateS = digitalRead(startB);
    while (stgtme1C > (millis() - countDownMillis))
    {
      int bStateS = digitalRead(startB);
      sensors.requestTemperatures();
      Celsius = sensors.getTempCByIndex(0);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(String("Temp: ") + String(Celsius));
      lcd.setCursor(0, 1);
      lcd.print(String("Time: ") + String(TimeLeft((stgtme1 * 60000 - (millis() - countDownMillis)))));
      if (Celsius >= stgtmp1)
      {
        digitalWrite(InfraredBulb, LOW);
        lcd.setCursor(0, 0);
        lcd.print("Max Temp");
      }
      else
      {
        digitalWrite(InfraredBulb, HIGH);
      }
      if (bStateS == 1)
      {
        buzzerBeep();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Initial Weight");
        stg1state = false;
        measureInitialWeight();
        delay(2000);
      }
    }
    if (bStateS == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Initial Weight");
      stg1state = false;
      measureInitialWeight();
      delay(2000);
    }
    buzzerBeep();
    delay(1000);
  }
}

/*Stage 1 Initial Weight Measurement*/
void measureInitialWeight()
{
  lcd.clear();
  while (moistCon_flagInit)
  {
    int bStateS = digitalRead(startB);
    LoadCell.update();
    float iWeight = LoadCell.getData();
    lcd.setCursor(0, 0);
    lcd.print("InitialWeight:");
    lcd.setCursor(0, 1);
    lcd.print(String(iWeight) + String("Grams"));
    delay(250);
    if (bStateS == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Initial W:");
      lcd.setCursor(0, 1);
      lcd.print(String(iWeight) + String("Grams"));
      delay(1000);
      buzzerBeep();
      moistCon_flagInit = false;
      systemStateSTG2();
    }
  }
}

/*Stage 2 heating process*/
void systemStateSTG2()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Stage 2 Heating");
  lcd.setCursor(0, 1);
  delay(1000);
  digitalWrite(Buzzer, HIGH);
  delay(100);
  digitalWrite(Buzzer, LOW);
  countDownMillis = millis();
  analogWrite(Fan, 255);
  while (stgtme2C > millis() - countDownMillis)
  {
    int bStateS = digitalRead(startB);
    sensors.requestTemperatures();
    Celsius = sensors.getTempCByIndex(0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(String("Temp: ") + String(Celsius));
    lcd.setCursor(0, 1);
    lcd.print(String("Time: ") + String(TimeLeft((stgtme2 * 60000 - (millis() - countDownMillis)))));
    if (Celsius >= stgtmp2)
    {
      digitalWrite(InfraredBulb, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Max Temp");
    }
    else
    {
      digitalWrite(InfraredBulb, HIGH);
    }
    if (bStateS == 1)
    {
      buzzerBeep();
      lcd.clear();
      lcd.setCursor(0, 0);
      systemStateSTG3();
      delay(2000);
    }
  }
}

/*Stage 3 heating process*/
void systemStateSTG3()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Stage 3 Heating");
  lcd.setCursor(0, 1);
  delay(1000);
  digitalWrite(Buzzer, HIGH);
  delay(100);
  digitalWrite(Buzzer, LOW);
  analogWrite(Fan, 255);
  long timeC = stgtme3C / 3;
  long time = stgtme3 / 3;
  int stg3State = 0;
  while (stg3State <= 2)
  {
    countDownMillis = millis();
    while (timeC > millis() - countDownMillis)
    {
      int bStateS = digitalRead(startB);
      sensors.requestTemperatures();
      Celsius = sensors.getTempCByIndex(0);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(String("Temp: ") + String(Celsius));
      lcd.setCursor(0, 1);
      lcd.print(String("Time: ") + String(TimeLeft((time * 60000 - (millis() - countDownMillis)))));
      if (Celsius >= stgtmp3)
      {
        digitalWrite(InfraredBulb, LOW);
      }
      else
      {
        digitalWrite(InfraredBulb, HIGH);
      }
      if (bStateS == 1)
      {
        buzzerBeep();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Skipping");
        systemStateSTG4();
        delay(2000);
      }
    }
    buzzerBeep();
    moistConCalc();
    stg3State++;
  }
  buzzerBeep();
  systemStateSTG4();
}

void systemStateSTG4()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Stage 2 Heating");
  lcd.setCursor(0, 1);
  delay(1000);
  digitalWrite(Buzzer, HIGH);
  delay(100);
  digitalWrite(Buzzer, LOW);
  analogWrite(Fan, 255);
  countDownMillis = millis();
  while (stgtme4C > millis() - countDownMillis)
  {
    int bStateS = digitalRead(startB);
    sensors.requestTemperatures();
    Celsius = sensors.getTempCByIndex(0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(String("Temp: ") + String(Celsius));
    lcd.setCursor(0, 1);
    lcd.print(String("Time: ") + String(TimeLeft((stgtme4 * 60000 - (millis() - countDownMillis)))));
    if (Celsius > stgtmp4)
    {
      digitalWrite(InfraredBulb, LOW);
    }
    else
    {
      digitalWrite(InfraredBulb, HIGH);
    }
    if (bStateS == 1)
    {
      buzzerBeep();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Skipping");
      delay(2000);
    }
  }
  buzzerBeep();
  moistConCalc4();
}

/*Moisture Content Calculation Function*/
void moistConCalc()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calculating");
  lcd.setCursor(0, 1);
  lcd.print("Moisture Content");
  delay(500);
  digitalWrite(Buzzer, HIGH);
  delay(100);
  digitalWrite(Buzzer, LOW);
  previousMillis = millis();
  while (millis() - previousMillis <= interWeight)
  {
    LoadCell.update();
    float fWeight = LoadCell.getData();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FinalWeight");
    lcd.setCursor(0, 1);
    lcd.print(String(fWeight) + String("Grams"));
    delay(250);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Computing");
  digitalWrite(Buzzer, HIGH);
  delay(250);
  digitalWrite(Buzzer, LOW);
  previousMillis = millis();
  MCC = ((iWeight - fWeight) / iWeight) * 100;
  while (millis() - previousMillis <= interWeight)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Moisture Content");
    lcd.setCursor(0, 1);
    lcd.print(String(MCC) + String("%"));
  }
  if (MCC <= MC)
  {
    buzzerBeep();
    endStateMachine();
  }
  return;
}

/*Moisture Content Calculation Function for Stage 4*/
void moistConCalc4()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calculating");
  lcd.setCursor(0, 1);
  lcd.print("Moisture Content");
  delay(500);
  digitalWrite(Buzzer, HIGH);
  delay(100);
  digitalWrite(Buzzer, LOW);
  previousMillis = millis();
  while (millis() - previousMillis <= interWeight)
  {
    LoadCell.update();
    float fWeight = LoadCell.getData();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FinalWeight");
    lcd.setCursor(0, 1);
    lcd.print(String(fWeight) + String("Grams"));
    delay(250);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Computing");
  digitalWrite(Buzzer, HIGH);
  delay(250);
  digitalWrite(Buzzer, LOW);
  previousMillis = millis();
  MCC = ((iWeight - fWeight) / iWeight) * 100;
  while (millis() - previousMillis <= interWeight)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Moisture Content");
    lcd.setCursor(0, 1);
    lcd.print(String(MCC) + String("%"));
  }
  if (MCC <= MC)
  {
    buzzerBeep();
    endStateMachine();
  }
  else
  {
    buzzerBeep();
    systemStateSTG4();
  }
}

/*End Machine*/
void endStateMachine()
{
  while (endState)
  {
    int bStateS = digitalRead(startB);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MContent Achieved");
    lcd.setCursor(0, 1);
    lcd.print(String(MCC) + String("%"));
    delay(250);
    digitalWrite(Buzzer, HIGH);
    delay(250);
    digitalWrite(Buzzer, LOW);
    if (bStateS == 1)
    {
      machineState();
    }
  }
}

/*Time Conversion and Countdown Function*/
String TimeLeft(unsigned long MsLeft)
{
  String Results;
  int M;
  int S;
  M = (long)MsLeft / 60000;
  if (M < 10)
  {
    Results = (String) "0" + M + ":";
  }
  else
  {
    Results = (String)M + ":";
  }
  S = (long)((MsLeft - M * 60000) / 1000);
  if (S < 10)
  {
    Results = (String)Results + "0" + S;
  }
  else
  {
    Results = (String)Results + S;
  }
  return Results;
}

/*Beeping the Buzzer*/
void buzzerBeep()
{
  digitalWrite(Buzzer, HIGH);
  delay(200);
  digitalWrite(Buzzer, LOW);
}

/*Display Variable Values*/
void dispVarSet()
{
  Serial.print("Stage1 " + String("Time:") + String(stgtme1) + String("Temp:") + String(stgtmp1));
  Serial.print("Stage2 " + String("Time:") + String(stgtme2) + String("Temp:") + String(stgtmp2));
  Serial.print("Stage3 " + String("Time:") + String(stgtme3) + String("Temp:") + String(stgtmp3));
  Serial.print("Stage4 " + String("Time:") + String(stgtme4) + String("Temp:") + String(stgtmp4));
  Serial.println("Stage1 " + String("TimeC:") + String(stgtme1C));
  Serial.print("Stage2 " + String("TimeC:") + String(stgtme2C));
  Serial.print("Stage3 " + String("TimeC:") + String(stgtme3C));
  Serial.print("Stage4 " + String("TimeC:") + String(stgtme4C));
}