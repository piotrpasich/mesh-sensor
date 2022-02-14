#ifndef LIGHT_H
#define LIGHT_H
#include <DHT.h>
#include "sensor.h"

class Light: public Sensor
{
  public:
   Light(uint8_t lightPin, uint8_t buttonPin, String id);
   String getMessage();
   void setup();
   void on();
   void off();
   void change();
   void execute(String name);
   void interrupt();
   uint8_t getInterruptPin();

 private:
  uint8_t lightPin;
  uint8_t buttonPin;
  String id;
  bool state = false;

  int str2int(String text);
};
#endif
