#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include <DHT.h>
#include "sensor.h"

class Temperature: public Sensor
{
  public:
   Temperature(uint8_t pin, uint8_t dhtType, String id);
   float readTemperature();
   float readHumidity();
   String getMessage();
   void setup();
   
   void execute(String name);
   void interrupt();
   uint8_t getInterruptPin();
 private:
  DHT dht;
  String id;
};
#endif
