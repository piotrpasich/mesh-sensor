#include "painlessMesh.h"
#include "temperature.h"
#include "env.h"

#define DHT_PIN     D8
#define DHT_TYPE    DHT11

Scheduler userScheduler;
painlessMesh  mesh;

DHT dht(DHT_PIN, DHT_TYPE);
Temperature temperature(DHT_PIN, DHT_TYPE, ID);

void sendMessage() ;

Task taskSendMessage( TASK_SECOND * 10 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = temperature.getMessage();
  Serial.println(msg);  
  mesh.sendBroadcast( msg );  
  taskSendMessage.setInterval( random( TASK_SECOND * 10, TASK_SECOND * 50 ));  
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.println("receivedCallback");
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
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
  temperature.setup();
  digitalWrite(BUILTIN_LED, LOW);
}

void loop() {
  mesh.update();
}
