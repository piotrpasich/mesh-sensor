#include "painlessMesh.h"
#include <DHT.h>
#include <ArduinoJson.h>
#include "temperature.h"

#define ID "1fb190ff-a400-42ee-9682-409b382a1317"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

#define DHT_PIN     D8
#define DHT_TYPE    DHT11

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

DHT dht(DHT_PIN, DHT_TYPE);
Temperature temperature(DHT_PIN, DHT_TYPE, ID);

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = temperature.getMessage();
  Serial.println(msg);  
  mesh.sendBroadcast( msg );  
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));  
}

// Needed for painless library
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
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
  dht.begin();
  digitalWrite(BUILTIN_LED, LOW);
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}
