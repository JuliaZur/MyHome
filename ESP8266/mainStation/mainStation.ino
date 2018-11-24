// Enable debug prints to serial monitor
#define MY_DEBUG

#define MY_GATEWAY_ESP8266

#define MY_WIFI_SSID "iPhone (Kuba)"
#define MY_WIFI_PASSWORD "MyHomeWifi"

#define MY_PORT 5003
#define MY_GATEWAY_MAX_CLIENTS 2

#define LIGHT_ON 1
#define LIGHT_OFF 0

// Enable and select radio type attached
#define MY_RADIO_NRF24


#include "MYSLog.h"
#include <ESP8266WiFi.h>
#include <MySensors.h>


#define VSN "v1.0"

// Define two generic nodes with a single child
#define MY_NODE_ID 0
#define MAINSTATION 0
#define FLOOR1 201
#define CHILD 1

MyMessage mPing(CHILD, V_VAR1);   //Ping message
MyMessage mPong(CHILD, V_VAR2);   //Pong message

void setup()
{
  //setNodeId(MAINSTATION);
}

void presentation()
{
  //present(CHILD, S_CUSTOM);  //

	//sendSketchInfo( nodeTypeAsCharRepresentation( getNodeId() ), VSN );
	//LOG(F("\n%sReady.\n"), nodeTypeAsCharRepresentation(getNodeId()));
}

void loop()
{
	if (Serial.available()) {
		byte inChar = Serial.read();
		uint8_t node = getNodeId();

		// Manual Test Mode
		if (inChar == 'y') {
			Serial.println("turning on light: floor 1");
			MyMessage turnON(1,V_TRIPPED);
      turnON.setDestination(FLOOR1);
      send(turnON.set(LIGHT_ON));
		} else if (inChar == 'n') {
			Serial.println("turning on light: floor 1");
      MyMessage turnOFF(1,V_TRIPPED);
      turnOFF.setDestination(FLOOR1);
      send(turnOFF.set(LIGHT_OFF));
		} else {
			Serial.println("Invalid input");
		}
	}
}

void receive(const MyMessage &message)
{

	/*LOG(F("Received %s from %s\n"), msgTypeAsCharRepresentation((mysensor_data)message.type),
	    nodeTypeAsCharRepresentation(message.sender));
*/
	//delay(250);
  Serial.print("received");
  Serial.println(message.sender);
	//sendPingOrPongResponse( message );
}

void sendPingOrPongResponse( MyMessage msg ){

	MyMessage response = (msg.type == V_VAR1 ? mPong : mPing);

	LOG(F("Sending %s to %s\n"), msgTypeAsCharRepresentation( (mysensor_data)response.type ),
	    nodeTypeAsCharRepresentation(msg.sender));

	// Set payload to current time in millis to ensure each message is unique
	response.set( (uint32_t)millis() );
	response.setDestination(msg.sender);
	send(response);
}

void setNodeId(byte nodeID)
{
	//LOG(F("Setting node id to: %i.\n***Please restart the node for changes to take effect.\n"), nodeID);
	hwWriteConfig(EEPROM_NODE_ID_ADDRESS, (byte)nodeID);
}

const char * msgTypeAsCharRepresentation( mysensor_data mType )
{
	return mType == V_VAR1 ? "Ping" : "Pong";
}

const char * nodeTypeAsCharRepresentation( uint8_t node )
{
	//return node == YING ? "Ying Node" : "Yang Node";
}
