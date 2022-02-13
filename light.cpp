#include "light.h"
#include <ArduinoJson.h>
int Light::str2int(String text) 
{
  int result = 0;
  for (int i = 0; i < text.length(); i++) {
    result += (int) text[i];
  }

  return result;
}

Light::Light(uint8_t u_lightPin, uint8_t u_buttonPin, String s_id): id(s_id), lightPin(u_lightPin), buttonPin(u_buttonPin)
{
  
}
String Light::getMessage()
{
  digitalWrite(BUILTIN_LED, HIGH);
  Serial.print("send message: ");
  
  StaticJsonDocument<200> document;    
  document["sensorType"] = "Light";
  document["id"] = id;

  document["state"] = state ? 1 : 0;
  String msg;
  serializeJson(document, msg);
  
  digitalWrite(BUILTIN_LED, LOW);
  return msg;
}
void Light::setup()
{
  pinMode(lightPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(lightPin, LOW);
}
void Light::execute(String dname) {
  int iname = str2int(dname);
  Serial.printf("Execute: %s: %i\n", dname.c_str(), iname);
  switch (iname) {
    case 221: 
      on();
      break;
    case 315: 
      off();
      break;
    case 658: 
      change();
      break;
  }
}
uint8_t Light::getInterruptPin() {
  return buttonPin;
}
void Light::on()
{
  state = true;
  digitalWrite(lightPin, HIGH);
}
void Light::off()
{
  state = false;
  digitalWrite(lightPin, LOW);
}
void Light::interrupt() 
{
  change();
}
void Light::change()
{
  if (state) {
    off();
  } else {
    on();
  }
}
