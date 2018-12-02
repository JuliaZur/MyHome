#define MY_DEBUG 

#define MY_RADIO_NRF24
#define MY_RF24_PA_LEVEL RF24_PA_LOW
#define MY_RF24_CE_PIN 10
#define MY_RF24_CS_PIN 9
#define MY_NODE_ID 201
#define MY_PARENT_NODE_ID 0
#define MY_PARENT_NODE_IS_STATIC

#include <SPI.h>
#include <MySensors.h>
#include "MYSLog.h"
#include <DHT.h>

#define VSN "v1.0"

// Define two generic nodes with a single child
#define main 200
#define NODE_ID 201
#define CHILD_LIGHT 1

#define DHT_DATA_PIN 8
#define LED_PIN 2

//V_TRIPPED
#define GET_LIGHT_STATUS 2
#define GET_TEMP 3
#define GET_HUM 4

//V_STATUS
#define LIGHT_ON 1
#define LIGHT_OFF 0

//V_TEMP
#define CHILD_TEMP 2

//V_HUM
#define CHILD_HUM 3

static const uint64_t UPDATE_INTERVAL = 60000;
long lightStatus;

float temp;
float hum;
bool metric = true;

MyMessage msgHum(CHILD_HUM, V_HUM);
MyMessage msgTemp(CHILD_TEMP, V_TEMP);
DHT dht;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);

  dht.setup(DHT_DATA_PIN); // set data pin of DHT sensor
  if (UPDATE_INTERVAL <= dht.getMinimumSamplingPeriod()) {
    Serial.println("Warning: UPDATE_INTERVAL is smaller than supported by the sensor!");
  }

  msgHum.setDestination(MY_PARENT_NODE_ID);
  msgTemp.setDesination(MY_PARENT_NODE_ID);
  
  // Sleep for the time of the minimum sampling period to give the sensor time to power up
  // (otherwise, timeout errors might occure for the first reading)
  sleep(dht.getMinimumSamplingPeriod());
}

void presentation() {
  //present(CHILD_LIGHT, S_BINARY);
  //present(CHILD_HUM, S_HUM);
  //present(CHILD_TEMP, S_TEMP);
  metric = getControllerConfig().isMetric;
}

void loop() {

  dht.readSensor(true);

  readTemp();
  readHum();
 
  send(msgHum.set(hum));
  send(msgTemp.set(temp));
  
  delay(UPDATE_INTERVAL*5);
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
          MyMessage msg(CHILD_LIGHT,V_TRIPPED);
          msg.setDestination(MY_PARENT_NODE_ID);
          lightStatus = digitalRead(LED_PIN);
          send(msg.set(lightStatus));
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
  if (isnan(temp)) {
    Serial.println("Failed reading temperature from DHT!");
  }
}

void readHum(){
  hum = dht.getHumidity();
  if(isnan(hum)) {
    Serial.println("Failed reading humidity from DHT!");
  }
}
