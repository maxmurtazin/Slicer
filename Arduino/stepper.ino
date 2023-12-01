#include <Ethernet.h>
#include <EthernetClient.h>
#include <Dns.h>
#include <Dhcp.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
const int dirPin = 7;
const int stepPin = 6;

//MOVE
// ST 1/8
const int MV50 = 10000 ; 
const int MV10 = 2000 ; 
const int MV1 = 200 ; 
const int MV01 = 20 ;  
const int MV005 = 10  ;
const int MV001 = 2 ;


//MOVE(MV10, LOW)
//MOVE(MV10, HIGH)

//MOVE(MV1, LOW)
//MOVE(MV1, HIGH)

//MOVE(MV01, LOW)
//MOVE(MV01, HIGH)


// ST 1/16
// const int MV50 = 20000 ; 
// const int MV10 = 4000 ; 
// const int MV1 =  400; 
// const int MV01 = 40 ;  
// const int MV005 = 20 ;
// const int MV001 = 4 ;


String readString;
int ledPin = 5;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xBE };

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "192.168.0.168"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "mosquitto"
#define AIO_KEY         "1234"


/************ Global State (you don't need to change this!) ******************/

//Set up the ethernet client
EthernetClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// You don't need to change anything below this line!
//define halt(s) { Serial.println(F( s )); while(1);  }

/****************************** Feeds ***************************************/



//Adafruit_MQTT_Subscribe stepper_up = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/printer/stepper_up");
//Adafruit_MQTT_Subscribe stepper_down = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/printer/stepper_down");


Adafruit_MQTT_Subscribe STEPPER_UPP = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/printer/STEPPER_UPP");
Adafruit_MQTT_Subscribe stepper_up = Adafruit_MQTT_Subscribe(&mqtt, STEPPER_UPP);
Adafruit_MQTT_Subscribe STEPPER_DOWNN = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/printer/STEPPER_DOWNN");
Adafruit_MQTT_Subscribe stepper_down = Adafruit_MQTT_Subscribe(&mqtt, STEPPER_DOWNN);


void setup(){

  // Объявить контакты как выходы
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT); 
  

    pinMode(ledPin, OUTPUT); //pin selected to control
    // Test LED
    digitalWrite(ledPin, HIGH); // set pin high
    delay(500);
    digitalWrite(ledPin, LOW); // set pin low
   
   


     Serial.begin(115200);
     Ethernet.begin(mac);
  delay(1000); //give the ethernet a second to initialize
   mqtt.subscribe(&stepper_down);
   mqtt.subscribe(&stepper_up);


     // ping the server to keep the mqtt connection alive
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }

  }


int MOVE_UP (int heihgt){ //
   // Установка направления вращения двигателя по часовой стрелке.
 
  digitalWrite(dirPin, HIGH); // по часовой
  // Медленное вращение двигателя
  for(int x = 0; x < heihgt; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
 // delay(5000); // Ждем 
}

int MOVE_DOWN (int heihgt){ //
   // Установка направления вращения двигателя по часовой стрелке.

  digitalWrite(dirPin, LOW); // по часовой
  // Медленное вращение двигателя
  for(int x = 0; x < heihgt; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
 // delay(5000); // Ждем
}


void loop(){
MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {

    if (subscription == &stepper_down) {
      Serial.print(F("stepper down: "));
      Serial.println((char *)stepper_down.lastread);

      if (strcmp((char *)stepper_down.lastread, "10") == 0) {
        MOVE_DOWN(MV10);
      }
      if (strcmp((char *)stepper_down.lastread, "1") == 0) {
        MOVE_DOWN(MV1);
      }
      if (strcmp((char *)stepper_down.lastread, "01") == 0) {
        MOVE_DOWN(MV01);
      }

    }


       if (subscription == &stepper_up) {
      Serial.print(F("stepper up: "));
      Serial.println((char *)stepper_up.lastread);

      if (strcmp((char *)stepper_up.lastread, "10") == 0) {
        MOVE_UP(MV10);
      }
      if (strcmp((char *)stepper_up.lastread, "1") == 0) {
        MOVE_UP(MV1);
      }
      if (strcmp((char *)stepper_up.lastread, "01") == 0) {
        MOVE_UP(MV01);
      }

    }

  delay(1000);
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