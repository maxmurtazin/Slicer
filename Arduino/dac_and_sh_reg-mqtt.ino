#include <Arduino_Helpers.h> // Include the Arduino Helpers library.
#include <AH/Hardware/ExtendedInputOutput/ShiftRegisterOut.hpp>
using namespace ExtIO; // Bring the ExtIO pin functions into your sketch

#include "Wire.h"
#include "DFRobot_MCP4725.h"

#include <Ethernet.h>
#include <EthernetClient.h>
#include <Dns.h>
#include <Dhcp.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// OUTPUT_VOLTAGE_DAC = 30;        // unit : mV
//int REG_OUTPUT_VOLTAGE_DAC = 0;
 int mV=30;
#define  REF_VOLTAGE    4850
DFRobot_MCP4725 DAC;


const pin_t latchPin = 3; // Pin connected to ST_CP of 74HC595 (violet_T)
const pin_t dataPin = 2;  // Pin connected to DS of 74HC595 (grey_D)
const pin_t clockPin = 4; // Pin connected to SH_CP of 74HC595 (blue_H)
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE };

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



Adafruit_MQTT_Subscribe stepper_up = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/printer/dac"); // mosquitto/printer/dac
Adafruit_MQTT_Subscribe stepper_down = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/printer/shreg"); //// mosquitto/printer/stepper_down




 

ShiftRegisterOut<256> sreg {dataPin, clockPin, latchPin, MSBFIRST};//количество пинов сдвигового регистра
//// SH REG
const pin_t k0 = sreg.pin(0); //1
const pin_t k1 = sreg.pin(1);
const pin_t k2 = sreg.pin(2);
const pin_t k3 = sreg.pin(3);

const pin_t k4 = sreg.pin(4); //2
const pin_t k5 = sreg.pin(5);
const pin_t k6 = sreg.pin(6);
const pin_t k7 = sreg.pin(7);

const pin_t k8 = sreg.pin(8); //3
const pin_t k9 = sreg.pin(9);
const pin_t k10 = sreg.pin(10);
const pin_t k11 = sreg.pin(11);

const pin_t k12 = sreg.pin(12); //4
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

const pin_t k16 = sreg.pin(0); //5
const pin_t k17 = sreg.pin(1);
const pin_t k18 = sreg.pin(2);
const pin_t k19 = sreg.pin(3);

const pin_t k20 = sreg.pin(4); //6
const pin_t k21 = sreg.pin(5);
const pin_t k22 = sreg.pin(6);
const pin_t k23 = sreg.pin(7);

const pin_t k24 = sreg.pin(8); //7
const pin_t k25 = sreg.pin(9);
const pin_t k26 = sreg.pin(10);
const pin_t k27 = sreg.pin(11);

const pin_t k28 = sreg.pin(12); //8
const pin_t k29 = sreg.pin(13);
const pin_t k30 = sreg.pin(14);
const pin_t k31 = sreg.pin(15);

const pin_t k32 = sreg.pin(0); //9
const pin_t k33 = sreg.pin(1);
const pin_t k34 = sreg.pin(2);
const pin_t k35 = sreg.pin(3);

const pin_t k36 = sreg.pin(4); //10
const pin_t k37 = sreg.pin(5);
const pin_t k38 = sreg.pin(6);
const pin_t k39 = sreg.pin(7);

const pin_t k40 = sreg.pin(8); //11
const pin_t k41 = sreg.pin(9);
const pin_t k42 = sreg.pin(10);
const pin_t k43 = sreg.pin(11);

const pin_t k44 = sreg.pin(12); //12
const pin_t k45 = sreg.pin(13);
const pin_t k46 = sreg.pin(14);
const pin_t k47 = sreg.pin(15);

const pin_t k48 = sreg.pin(0); //13
const pin_t k49 = sreg.pin(1);
const pin_t k50 = sreg.pin(2);
const pin_t k51 = sreg.pin(3);

const pin_t k52 = sreg.pin(4); //14
const pin_t k53 = sreg.pin(5);
const pin_t k54 = sreg.pin(6);
const pin_t k55 = sreg.pin(7);

const pin_t k56 = sreg.pin(8); //15
const pin_t k57 = sreg.pin(9);
const pin_t k58 = sreg.pin(10);
const pin_t k59 = sreg.pin(11);

const pin_t k60 = sreg.pin(12); //16
const pin_t k61 = sreg.pin(13);
const pin_t k62 = sreg.pin(14);
const pin_t k63 = sreg.pin(15);
/////////////////////////////////////////////////
const pin_t k0 = sreg.pin(0); //17
const pin_t k1 = sreg.pin(1);
const pin_t k2 = sreg.pin(2);
const pin_t k3 = sreg.pin(3);

const pin_t k4 = sreg.pin(4); //18
const pin_t k5 = sreg.pin(5);
const pin_t k6 = sreg.pin(6);
const pin_t k7 = sreg.pin(7);

const pin_t k8 = sreg.pin(8); //19
const pin_t k9 = sreg.pin(9);
const pin_t k10 = sreg.pin(10);
const pin_t k11 = sreg.pin(11);

const pin_t k12 = sreg.pin(12); //20
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

const pin_t k0 = sreg.pin(0); //21
const pin_t k1 = sreg.pin(1);
const pin_t k2 = sreg.pin(2);
const pin_t k3 = sreg.pin(3);

const pin_t k4 = sreg.pin(4); //22
const pin_t k5 = sreg.pin(5);
const pin_t k6 = sreg.pin(6);
const pin_t k7 = sreg.pin(7);

const pin_t k8 = sreg.pin(8); //23
const pin_t k9 = sreg.pin(9);
const pin_t k10 = sreg.pin(10);
const pin_t k11 = sreg.pin(11);

const pin_t k12 = sreg.pin(12); //24
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

const pin_t k0 = sreg.pin(0); //25
const pin_t k1 = sreg.pin(1);
const pin_t k2 = sreg.pin(2);
const pin_t k3 = sreg.pin(3);

const pin_t k4 = sreg.pin(4); //26
const pin_t k5 = sreg.pin(5);
const pin_t k6 = sreg.pin(6);
const pin_t k7 = sreg.pin(7);

const pin_t k8 = sreg.pin(8); //27
const pin_t k9 = sreg.pin(9);
const pin_t k10 = sreg.pin(10);
const pin_t k11 = sreg.pin(11);

const pin_t k12 = sreg.pin(12); //28
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

const pin_t k0 = sreg.pin(0); //29
const pin_t k1 = sreg.pin(1);
const pin_t k2 = sreg.pin(2);
const pin_t k3 = sreg.pin(3);

const pin_t k4 = sreg.pin(4); //30
const pin_t k5 = sreg.pin(5);
const pin_t k6 = sreg.pin(6);
const pin_t k7 = sreg.pin(7);

const pin_t k8 = sreg.pin(8); //31
const pin_t k9 = sreg.pin(9);
const pin_t k10 = sreg.pin(10);
const pin_t k11 = sreg.pin(11);

const pin_t k12 = sreg.pin(12); //32
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

const pin_t k0 = sreg.pin(0); //33
const pin_t k1 = sreg.pin(1);
const pin_t k2 = sreg.pin(2);
const pin_t k3 = sreg.pin(3);

const pin_t k4 = sreg.pin(4); //34
const pin_t k5 = sreg.pin(5);
const pin_t k6 = sreg.pin(6);
const pin_t k7 = sreg.pin(7);

const pin_t k8 = sreg.pin(8); //35
const pin_t k9 = sreg.pin(9);
const pin_t k10 = sreg.pin(10);
const pin_t k11 = sreg.pin(11);

const pin_t k12 = sreg.pin(12); //36
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

const pin_t k0 = sreg.pin(0); //37
const pin_t k1 = sreg.pin(1);
const pin_t k2 = sreg.pin(2);
const pin_t k3 = sreg.pin(3);

const pin_t k4 = sreg.pin(4); //38
const pin_t k5 = sreg.pin(5);
const pin_t k6 = sreg.pin(6);
const pin_t k7 = sreg.pin(7);

const pin_t k8 = sreg.pin(8); //39
const pin_t k9 = sreg.pin(9);
const pin_t k10 = sreg.pin(10);
const pin_t k11 = sreg.pin(11);

const pin_t k12 = sreg.pin(12); //40
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

const pin_t k0 = sreg.pin(0); //41
const pin_t k1 = sreg.pin(1);
const pin_t k2 = sreg.pin(2);
const pin_t k3 = sreg.pin(3);

const pin_t k4 = sreg.pin(4); //42
const pin_t k5 = sreg.pin(5);
const pin_t k6 = sreg.pin(6);
const pin_t k7 = sreg.pin(7);

const pin_t k8 = sreg.pin(8); //43
const pin_t k9 = sreg.pin(9);
const pin_t k10 = sreg.pin(10);
const pin_t k11 = sreg.pin(11);

const pin_t k12 = sreg.pin(12); //44
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

const pin_t k0 = sreg.pin(0); //45
const pin_t k1 = sreg.pin(1);
const pin_t k2 = sreg.pin(2);
const pin_t k3 = sreg.pin(3);

const pin_t k4 = sreg.pin(4); //46
const pin_t k5 = sreg.pin(5);
const pin_t k6 = sreg.pin(6);
const pin_t k7 = sreg.pin(7);

const pin_t k8 = sreg.pin(8); //47
const pin_t k9 = sreg.pin(9);
const pin_t k10 = sreg.pin(10);
const pin_t k11 = sreg.pin(11);

const pin_t k12 = sreg.pin(12); //48
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

const pin_t k0 = sreg.pin(0); //49
const pin_t k1 = sreg.pin(1);
const pin_t k2 = sreg.pin(2);
const pin_t k3 = sreg.pin(3);

const pin_t k4 = sreg.pin(4); //50
const pin_t k5 = sreg.pin(5);
const pin_t k6 = sreg.pin(6);
const pin_t k7 = sreg.pin(7);

const pin_t k8 = sreg.pin(8); //51
const pin_t k9 = sreg.pin(9);
const pin_t k10 = sreg.pin(10);
const pin_t k11 = sreg.pin(11);

const pin_t k12 = sreg.pin(12); //52
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

const pin_t k0 = sreg.pin(0); //53
const pin_t k1 = sreg.pin(1);
const pin_t k2 = sreg.pin(2);
const pin_t k3 = sreg.pin(3);

const pin_t k4 = sreg.pin(4); //54
const pin_t k5 = sreg.pin(5);
const pin_t k6 = sreg.pin(6);
const pin_t k7 = sreg.pin(7);

const pin_t k8 = sreg.pin(8); //55
const pin_t k9 = sreg.pin(9);
const pin_t k10 = sreg.pin(10);
const pin_t k11 = sreg.pin(11);

const pin_t k12 = sreg.pin(12); //56
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

const pin_t k0 = sreg.pin(0); //57
const pin_t k1 = sreg.pin(1);
const pin_t k2 = sreg.pin(2);
const pin_t k3 = sreg.pin(3);

const pin_t k4 = sreg.pin(4); //58
const pin_t k5 = sreg.pin(5);
const pin_t k6 = sreg.pin(6);
const pin_t k7 = sreg.pin(7);

const pin_t k8 = sreg.pin(8); //59
const pin_t k9 = sreg.pin(9);
const pin_t k10 = sreg.pin(10);
const pin_t k11 = sreg.pin(11);

const pin_t k12 = sreg.pin(12); //60
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

const pin_t k12 = sreg.pin(12); //61
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

const pin_t k12 = sreg.pin(12); //62
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);


const pin_t k12 = sreg.pin(12); //63
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

const pin_t k12 = sreg.pin(12); //64
const pin_t k13 = sreg.pin(13);
const pin_t k14 = sreg.pin(14);
const pin_t k15 = sreg.pin(15);

////////////////////////////////////////////
 
int Key_On (int KeyNumber) { //on
digitalWrite(KeyNumber, HIGH)
}

int Key_Of (int KeyNumber) { // of
digitalWrite(KeyNumber, LOW)
}


void setup()
{
   sreg.begin(); 
//DAC 

  DAC.init(0x62, REF_VOLTAGE);

  Serial.begin(115200);
   Ethernet.begin(mac);
  delay(1000); //give the ethernet a second to initialize
   mqtt.subscribe(&dac);
   mqtt.subscribe(&shreg);


     // ping the server to keep the mqtt connection alive
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
 
}


int REG_VOLT (int mV){ //
  DAC.outputVoltage(mV);
  
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






void loop()
{
MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {

 // check if its the slider feed
    if (subscription == &shreg {
      Serial.print(F("shreg: "));
      Serial.println((char *)shreg.lastread);
      uint16_t key_val = atoi((char *)shreg.lastread);  // convert to a number
      Key_On(key_val);
    }


    if (subscription == &dac {
      Serial.print(F("dac: "));
      Serial.println((char *)dac.lastread);
      uint16_t mV = atoi((char *)dac.lastread);  // convert to a number
      REG_VOLT(mV);;
    }
}



  delay(500);

  }
///////////////////////////////////////////////////////////////////////
