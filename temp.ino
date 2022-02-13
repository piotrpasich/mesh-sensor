#include "painlessMesh.h"
#include "temperature.h"
#include "env.h"
#include "light.h"
#include <ArduinoJson.h>

#define DHT_PIN     D8
#define DHT_TYPE    DHT11

#define LIGHT_PIN D0
#define BUTTON_PIN D8

Scheduler userScheduler;
painlessMesh  mesh;

//DHT dht(DHT_PIN, DHT_TYPE);
//Temperature temperature(DHT_PIN, DHT_TYPE, ID);
Light light(LIGHT_PIN, BUTTON_PIN, ID);

void sendMessage() ;

Task taskSendMessage( TASK_SECOND * 10 , TASK_FOREVER, &sendMessage );

void sendMessage() {
//  String msg = temperature.getMessage();
  String msg = light.getMessage();
  Serial.printf("Send Message %s", msg.c_str());  
  mesh.sendBroadcast( msg );  
  taskSendMessage.setInterval( random( TASK_SECOND * 10, TASK_SECOND * 50 ));  
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.println("=========================================================");
  Serial.print("receivedCallback");
  Serial.printf("startHere: Received from %u msg='%s'\n", from, msg.c_str());
  String json = msg.c_str();

  StaticJsonDocument<2000> doc;
  
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  
  const char* messageType = doc["type"];
  const char* id = doc["id"];
  const char* action = doc["action"];
  const String sMessageType(messageType);
  const String sId(id);
  const String sAction(action);
  const String sID(ID);
  if (sId != sID || sMessageType != "incoming") {
    Serial.printf("\nMessage to another sensor %s : %s | %s\n", sId.c_str(), sID.c_str(), sMessageType.c_str());
    return;
  }

  light.execute(sAction);
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void ICACHE_RAM_ATTR interrupt ()
{
  light.interrupt();
  sendMessage();
}

void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, HIGH);
  mesh.setDebugMsgTypes( ERROR | STARTUP ); 

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
  //temperature.setup();
  light.setup();
  //light.on();
  attachInterrupt(digitalPinToInterrupt(light.getInterruptPin()), interrupt, FALLING);
  digitalWrite(BUILTIN_LED, LOW);
}

void loop() {
  mesh.update();
}
