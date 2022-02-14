#ifndef SENSOR_H
#define SENSOR_H

class Sensor
{
  public:
   virtual String getMessage();
   virtual void setup();
   virtual void execute(String name);
   virtual void interrupt();
   virtual uint8_t getInterruptPin();
};
#endif
