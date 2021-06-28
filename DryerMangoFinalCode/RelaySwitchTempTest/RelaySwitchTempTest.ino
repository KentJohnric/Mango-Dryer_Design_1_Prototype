#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS A0
float Celsius = 0;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  pinMode(A1, OUTPUT);
  Serial.begin(9600);
  Serial.print("On and Off Testing");
  sensors.begin();
  digitalWrite(13, HIGH);
}

void loop() {
  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);
  Serial.println(sensors.getTempCByIndex(0));
  if(Celsius <= 60){
    digitalWrite(A1, HIGH);
    Serial.println("Relay On");
  }
  else{
    digitalWrite(A1, LOW);
    Serial.println("Relay Off");
  }
  delay(100);
}
