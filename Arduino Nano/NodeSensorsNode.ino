
#define MY_DEBUG 

#define MY_RADIO_NRF24
#define MY_RF24_PA_LEVEL RF24_PA_LOW
#define MY_RF24_CE_PIN 10
#define MY_RF24_CS_PIN 9
#define MY_NODE_ID 201
#define MY_PARENT_NODE_ID 0
#define MY_PARENT_NODE_IS_STATIC

#include <MySensors.h>
#include "MYSLog.h"

#define VSN "v1.0"

// Define two generic nodes with a single child
#define main 200
#define NODE_ID 201
#define CHILD_LIGHT 1

#define LIGHT_ON 1
#define LIGHT_OFF 0

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);

}

void presentation() {
  //present(CHILD_LIGHT, S_BINARY);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
     LOG("Wait for msg\n");
     delay(5000);
   }
}

void receive(const MyMessage &message){
   if(message.type == V_TRIPPED){
      Serial.println(message.getString());
   } else{
      Serial.println("unnknown message");
   }
}
