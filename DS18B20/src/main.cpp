//#include <LiquidCrystal.h>
#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS A0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
   
void setup()
{
  sensors.begin();
  Serial.begin(9600);
}
   
void loop()
{
  sensors.requestTemperatures(); //发送获取温度的命令
  
  Serial.println(sensors.getTempCByIndex(0)); //获取温度
//  delay(1000);

  delay(1000);
}