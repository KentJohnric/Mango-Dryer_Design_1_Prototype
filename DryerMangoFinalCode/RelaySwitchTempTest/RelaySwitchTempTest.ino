#include <OneWire.h>
#include <DallasTemperature.h>
#define relay1 A1
#define ONE_WIRE_BUS A0
float Celsius = 0;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  pinMode(relay1, OUTPUT);
  Serial.begin(9600);
  Serial.print("On and Off Testing");
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);
  if(Celsius <= 60){
    digitalWrite(relay1, HIGH);
  }
  else{
    digitalWrite(relay1, LOW);
  }
}
