#include <ESP8266WiFi.h>

void setup() {
    //WiFi.softAPdisconnect(true);  //1
    //WiFi.disconnect(true);        //1
    ESP.eraseConfig();            //2
    ESP.reset();                  //2
}

void loop() {
  // put your main code here, to run repeatedly:

}
