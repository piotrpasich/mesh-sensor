#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include <DHT.h>

class Temperature 
{
  public:
   Temperature(uint8_t pin, uint8_t dhtType, String id);
   float readTemperature();
   float readHumidity();
   String getMessage();
   void setup();

 private:
  DHT dht;
  String id;
};
#endif
