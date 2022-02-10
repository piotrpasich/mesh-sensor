#include "temperature.h"
#include <DHT.h>
#include <ArduinoJson.h>
#define MIN_INTERVAL 5000

Temperature::Temperature(uint8_t pin, uint8_t dhtType, String s_id): dht(pin, dhtType), id(s_id)
{
 Serial.println("Temperature initialized");
}

String Temperature::getMessage()
{
  digitalWrite(BUILTIN_LED, HIGH);
  Serial.println("send message");
  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();

  Serial.print(temperature);
  Serial.print("|");
  Serial.println(humidity);
  StaticJsonDocument<200> document;    
  document["sensorType"] = "Temperature";
  document["id"] = id;

  if (!isnan(temperature) && !isnan(humidity)) {    
    document["temperature"] = temperature;
    document["humidity"] = humidity;
  } else {
    Serial.println("One of values is null");
  }
  digitalWrite(BUILTIN_LED, LOW);
  String msg;
  serializeJson(document, msg);
  
  return msg;
}
void Temperature::setup()
{
  dht.begin();
}
float Temperature::readTemperature()
{
  return dht.readTemperature();
}
float Temperature::readHumidity()
{
  return dht.readHumidity();
}
