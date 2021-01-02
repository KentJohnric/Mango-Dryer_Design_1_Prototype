#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 5
#define relay 3
#define startB 11
#define logicbE 12
#define logicbF 13
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27,16,2);
float MaxTemp = 60;
float Celcius=0;
float Fahrenheit=0;
int bstateE = 0;
int bstateF = 0;
int bstateS = 0;
void setup(void)
{
  pinMode(relay,OUTPUT);
  pinMode(logicbE, INPUT);
  pinMode(logicbF, INPUT);
  pinMode(startB, INPUT);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  sensors.begin();
}

void loop(void)
{ 
  lcd.setCursor(0,0);
  lcd.print("Press Start");
  lcd.setCursor(0,1);
  lcd.print("to Heat!");
  delay(1000);
  lcd.clear();
  button();
}
void heating(){
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Serial.println(" C  ");
  Serial.print(Celcius);
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(Celcius);
  if(Celcius >= MaxTemp){
    digitalWrite(relay, HIGH);
    lcd.setCursor(0,1);
    lcd.print("Max Temp ");
  }
  else{
    digitalWrite(relay, LOW);
    lcd.setCursor(0,1);
    lcd.print("Heating! ");
  }
  delay(1000);
}
void button(){
  bstateE = digitalRead(logicbE);
  bstateF = digitalRead(logicbF);
  bstateS = digitalRead(startB);
  if(bstateE == LOW && bstateF == HIGH){
    Serial.print("Down Button pressed");
    Serial.println();
  }
  else if(bstateE == HIGH && bstateF == LOW){
    Serial.print("Select Button pressed");
    Serial.println();
  }
  else if(bstateE == HIGH && bstateF == HIGH){
    Serial.print("Up Button pressed");
    Serial.println();
  }
  else if(bstateE == LOW && bstateF == LOW){
    Serial.println();
  }
  else if(bstateS == HIGH){
    heating();
  }
}
