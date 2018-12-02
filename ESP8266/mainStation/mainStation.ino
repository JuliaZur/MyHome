// Enable debug prints to serial monitor
#define MY_DEBUG

#define MY_GATEWAY_ESP8266

#define MY_WIFI_SSID "iPhone (Kuba)"
#define MY_WIFI_PASSWORD "MyHomeWifi"

//#define MY_HOSTNAME "myhome.local"

#define MY_PORT 5003
#define MY_GATEWAY_MAX_CLIENTS 2


// V_TRIPPED
#define LIGHT_ON 1
#define LIGHT_OFF 0
#define GET_LIGHT_STATUS 2
#define GET_TEMPERATURE 3
#define GET_HUMIDITY 4


// Enable and select radio type attached
#define MY_RADIO_NRF24


#include "MYSLog.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <MySensors.h>


#include <ESP8266WebServer.h>
ESP8266WebServer server(80);


#define VSN "v1.0"

// Define two generic nodes with a single child
#define MY_NODE_ID 0
#define MAINSTATION 0
#define FLOOR_1 201
#define FLOOR_2 202
#define CHILD 1

// FLOOR1
float floor1Temp = 0;
float floor1Hum  = 0;
int floor1Light  = 0;

// FLOOR 2
float floor2Temp = 0;
float floor2Hum  = 0;
int floor2Light  = 0;



void setup()
{
  setUpWebServer();
}

void setUpWebServer(){
  if(MDNS.begin("myhome")){
      Serial.println("MDNS started");
  }
  
  server.on("/", HandleRoot);
  server.on("/floor1ON", floor1LightOn);
  server.on("/floor1OFF", floor1LightOff);
  server.on("/floor2ON", floor2LightOn);
  server.on("/floor2OFF", floor2LightOff);

  //server.onNotFound( HandleNotFound );
  server.begin();
  Serial.println("WebServer started...");
}

void HandleRoot(){
  String json = "{\"FLOOR1\":{";
  json += "\"temperature\":";
  json += floor1Temp;
  json += ",\"humidity\":";
  json += floor1Hum;
  json += ",\"light\":";
  json += floor1Light;

  json += "},\"FLOOR2\":{";
  json += "\"temperature\":";
  json += floor2Temp;
  json += ",\"humidity\":";
  json += floor2Hum;
  json += ",\"light\":";
  json += floor2Light;
  
  json += "}}";

  server.send(200, "text/json", json);
}

void floor1LightOn(){
  Serial.println("turning on light: floor 1");
  MyMessage turnON(1,V_TRIPPED);
  turnON.setDestination(FLOOR_1);
  send(turnON.set(LIGHT_ON));
  HandleRoot();
}

void floor1LightOff(){
  Serial.println("turning off light: floor 1");
  MyMessage turnOFF(1,V_TRIPPED);
  turnOFF.setDestination(FLOOR_1);
  send(turnOFF.set(LIGHT_OFF));
  HandleRoot();
}

void floor2LightOn(){
  Serial.println("turning on light: floor 2");
  MyMessage turnON(1,V_TRIPPED);
  turnON.setDestination(FLOOR_2);
  send(turnON.set(LIGHT_ON));
  HandleRoot();
}
void floor2LightOff(){
  Serial.println("turning off light: floor 2");
  MyMessage turnOFF(1,V_TRIPPED);
  turnOFF.setDestination(FLOOR_2);
  send(turnOFF.set(LIGHT_OFF));
  HandleRoot();
}


void presentation()
{
  //present(CHILD, S_CUSTOM);  //

	//sendSketchInfo( nodeTypeAsCharRepresentation( getNodeId() ), VSN );
	//LOG(F("\n%sReady.\n"), nodeTypeAsCharRepresentation(getNodeId()));
}

void loop()
{
  server.handleClient();
	if (Serial.available()) {
		byte inChar = Serial.read();
		uint8_t node = getNodeId();

		// Manual Test Mode
		if (inChar == 'y') {
			Serial.println("turning on light: floor 1");
			MyMessage turnON(1,V_TRIPPED);
      turnON.setDestination(FLOOR_1);
      send(turnON.set(LIGHT_ON));
		} else if (inChar == 'n') {
			Serial.println("turning off light: floor 1");
      MyMessage turnOFF(1,V_TRIPPED);
      turnOFF.setDestination(FLOOR_1);
      send(turnOFF.set(LIGHT_OFF));
		} else if (inChar == 'g') {
      Serial.println("getting Floor 1 light status");
      MyMessage lightStatus(1,V_TRIPPED);
      lightStatus.setDestination(FLOOR_1);
      send(lightStatus.set(GET_LIGHT_STATUS));
		} else if (inChar == 't') {
      Serial.println("getting Floor 1 temperature");
      MyMessage getTemp(1,V_TRIPPED);
      getTemp.setDestination(FLOOR_1);
      send(getTemp.set(GET_TEMPERATURE));
    } else if (inChar == 'h') {
      Serial.println("getting Floor 1 humidity");
      MyMessage getHum(1,V_TRIPPED);
      getHum.setDestination(FLOOR_1);
      send(getHum.set(GET_HUMIDITY));
    } else {
			//Serial.println("Invalid input");
		}
	}
}

void receive(const MyMessage &message)
{
  if(message.type == V_TRIPPED){    //light
    if(message.sender == FLOOR_1){
      Serial.print("Floor 1 Light: ");
      if(message.getLong() == LIGHT_ON){
        Serial.println("ON");
      } else if(message.getLong() == LIGHT_OFF){
        Serial.println("OFF");
      } else{
        Serial.println("UNKNOWN");
      }
    } else if(message.sender == FLOOR_2){
      Serial.print("Floor 2 Light: ");
      if(message.getLong() == LIGHT_ON){
        Serial.println("ON");
      } else if(message.getLong() == LIGHT_OFF){
        Serial.println("OFF");
      } else{
        Serial.println("UNKNOWN");
      }
    } else{
      Serial.println("unknown light msg");
    } 
  } else if(message.type == V_TEMP){  //temperature
    if(message.sender == FLOOR_1){
      Serial.print("Floor 1 temperature: ");
      Serial.print(message.getFloat());
      Serial.println("°C");
    } else if(message.sender == FLOOR_2){
      Serial.print("Floor 2 temperature: ");
      Serial.print(message.getFloat());
      Serial.println("°C");
    } 
  } else if(message.type == V_HUM){  //temperature
    if(message.sender == FLOOR_1){
      Serial.print("Floor 1 humidity: ");
      Serial.print(message.getFloat());
      Serial.println("%");
    } else if(message.sender == FLOOR_2){
      Serial.print("Floor 2 humidity: ");
      Serial.print(message.getFloat());
      Serial.println("%");
    } 
  }
}
