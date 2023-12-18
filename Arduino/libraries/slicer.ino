#include "Wire.h"
#include "DFRobot_MCP4725.h"

#include <Ethernet.h>
#include <EthernetClient.h>
#include <Dns.h>
#include <Dhcp.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE };

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "192.168.0.168"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "mosquitto"
#define AIO_KEY         "1234"
void dlinna(String str){
  Serial.println(str.length());
};

/************ Global State (you don't need to change this!) ******************/

//Set up the ethernet client
EthernetClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// You don't need to change anything below this line!
//define halt(s) { Serial.println(F( s )); while(1);  }

/****************************** Feeds ***************************************/
Adafruit_MQTT_Subscribe slice = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/printer/slice"); // mosquitto/printer/slicer

void setup(){
  Serial.begin(115200);
  Ethernet.begin(mac);
  delay(1000); //give the ethernet a second to initialize
  mqtt.subscribe(&slice);
  // ping the server to keep the mqtt connection alive
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
}
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}

void loop(){
MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
   if (subscription == &slice) {
      // Serial.print(F("slice: "));
      String a = ((char *)slice.lastread);
      Serial.println(a);
      dlinna(a);
      // uint16_t key_val = atoi((char *)shreg.lastread);  // convert to a number
    }
  }
  delay(500);
}
///////////////////////////////////////////////////////////////////////
