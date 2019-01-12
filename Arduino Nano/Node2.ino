#define MY_DEBUG 

#define MY_RADIO_NRF24
#define MY_RF24_PA_LEVEL RF24_PA_LOW
#define MY_RF24_CE_PIN 10
#define MY_RF24_CS_PIN 9
#define MY_NODE_ID 202
#define MY_PARENT_NODE_ID 0
#define MY_PARENT_NODE_IS_STATIC

#include <SPI.h>
#include <MySensors.h>
#include "MYSLog.h"
#include <DHT.h>

#define VSN "v1.0"

#define CHILD_LIGHT 1
#define CHILD_TEMP 2
#define CHILD_HUM 3

#define DHT_DATA_PIN 8
#define LED_PIN 2

//V_TRIPPED
#define LIGHT_OFF 0
#define LIGHT_ON 1
#define GET_LIGHT_STATUS 2
#define GET_TEMP 3
#define GET_HUM 4

static const uint64_t UPDATE_INTERVAL = 60000;
long lightStatus;
int temp;
int hum;
long previousTime = 0;

MyMessage msgHum(CHILD_HUM, V_HUM);
MyMessage msgTemp(CHILD_TEMP, V_TEMP);
MyMessage msgLight(CHILD_LIGHT,V_TRIPPED);
DHT dht;

void setup() {
  
  pinMode(LED_PIN, OUTPUT);

  dht.setup(DHT_DATA_PIN); // data pin of DHT sensor
  
  if (UPDATE_INTERVAL <= dht.getMinimumSamplingPeriod()) {
    Serial.println("Warning: UPDATE_INTERVAL is smaller than supported by the sensor!");
  }

  msgHum.setDestination(MY_PARENT_NODE_ID);
  msgTemp.setDestination(MY_PARENT_NODE_ID);
  msgLight.setDestination(MY_PARENT_NODE_ID);

}

void loop() {
 
  unsigned long currentTime = millis();

  if(currentTime-previousTime >= UPDATE_INTERVAL){
      readTemp();
      readHum();
      previousTime = millis();
  }
}

void receive(const MyMessage &message){
   if(message.type == V_TRIPPED){
      Serial.println(message.getString());
      Serial.print("received: ");
      Serial.println(message.getLong());
      if (message.getLong() == LIGHT_ON){
          turnLightOn();
      } else if (message.getLong() == LIGHT_OFF){
          turnLightOff();
      } else if (message.getLong() == GET_LIGHT_STATUS){
          lightStatus = digitalRead(LED_PIN);
          send(msgLight.set(lightStatus));
      } else if (message.getLong() == GET_TEMP){
          send(msgTemp.set(temp));
      } else if (message.getLong() == GET_HUM){
          send(msgHum.set(hum));
      }
   } else{
      Serial.println("unnknown message");
   }
}

void turnLightOn(){
   digitalWrite(LED_PIN,HIGH);
}

void turnLightOff(){
   digitalWrite(LED_PIN,LOW);
}

void readTemp(){
  temp = dht.getTemperature();
  Serial.println(temp);
  if (isnan(temp)) {
    Serial.println("Failed reading temperature from DHT!");
  } else {
    send(msgTemp.set(temp));
  }
}

void readHum(){
  hum = dht.getHumidity();
  Serial.print(hum);
  Serial.println("%");
  if(isnan(hum)) {
    Serial.println("Failed reading humidity from DHT!");
  } else{
    send(msgHum.set(hum));
  }
}
