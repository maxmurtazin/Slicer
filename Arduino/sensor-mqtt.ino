#include <EEPROM.h>
#include <CN0349.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1015 ads; 
#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;
CN0349 CT;  //initialize CT sensor

#include <SPI.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include <Ethernet.h>
#include <EthernetClient.h>
#include <Dns.h>
#include <Dhcp.h>
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};



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
#define halt(s) { Serial.println(F( s )); while(1);  }


/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish ph = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/printer/ph");//mosquitto/printer/ph"
Adafruit_MQTT_Publish cond = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/printer/cond");
Adafruit_MQTT_Publish curr = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/printer/curr");
Adafruit_MQTT_Publish volt = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/printer/volt");

// Setup a feed called 'onoff' for subscribing to changes.
//Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

//For saving calibration gain factors and offsets:
template <class T> int EEPROM_writeAnything(int ee, const T& value) { //saves value into memory
  const uint8_t* p = (const uint8_t*)(const void*)&value;
  uint16_t i;
  for (i = 0; i < sizeof(value); i++)
    EEPROM.write(ee++, *p++);
  return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value) {  //retrieves value from memory
  uint8_t* p = (uint8_t*)(void*)&value;
  uint16_t i;
  for (i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(ee++);
  return i;
}

void calibrateCN0349(char state) { //find, and save gain factors, and offsets. See AD5934 and CN-0349 datasheets for process
  //CN-0349 reference:
  //      Rcal(ohms) | Rfb(ohms)  |MUXChannels
  //RTD:   R3(100)     R9(100)      4,1
  //High1: R3(100)     R9(100)      4,1
  //High2: R4(1000)    R9(100)      5,1
  //Low1:  R4(1000)    R8(1000)     5,2
  //Low2:  R7(10000)   R8(1000)     6,2
  float H1Tmag, H2mag, L1mag, L2mag = 0;
  float YL, YH, NH, NL, GF_low, NOS_low, GF_high, NOS_high = 0;
  //reference:                      //three point calibration
  //Rcal = {R3,R4,R7}={100,1000,10000}
  if (state == 'L') {
    //LOW MODE:
    L1mag =  CT.calibrate(R4, R4);    //Low range 1 //was 1000 1000
    Serial.println(L1mag, 8);
    Serial.println(F("Low Magnitude 1 Found"));
    L2mag =  CT.calibrate(R7, R4);   //Low range 2
    Serial.println(L2mag, 8);
    Serial.println(F("Low Magnitude 2 Found"));

    YL = 1 / R7;   //low admittance
    YH = 1 / R4;   //high admittance
    NL = L2mag;    //Magnitudes
    NH = L1mag;
    GF_low = (YH - YL) / (NH - NL); //interpolation
    NOS_low = NH - (YH) / GF_low;   //calculate offset
    EEPROM_writeAnything(GF_low_addr, GF_low); //save values
    EEPROM_writeAnything(NOS_low_addr, NOS_low);
    EEPROM_writeAnything(calibState_addr, 2);
  }
  //HIGH MODE:
  if (state == 'H') {
    H1Tmag = CT.calibrate(R3, R9);     //High range 1
    Serial.println(H1Tmag, 8);
    Serial.println(F("High Magnitude 1 Found"));
    H2mag = CT.calibrate(R4, R9);     //High range 2
    Serial.println(H2mag, 8);
    Serial.println(F("High Magnitude 2 Found"));

    YL = 1 / R4;   //low admittance
    YH = 1 / R3;     //high admittance
    NL = H2mag;    //Magnitudes
    NH = H1Tmag;

    GF_high = (YH - YL) / (NH - NL);   //interpolation
    NOS_high = NH - YH / GF_high;      //calculate offset
    EEPROM_writeAnything(GF_high_addr, GF_high);  //save values in memory
    EEPROM_writeAnything(NOS_high_addr, NOS_high);
    EEPROM_writeAnything(calibState_addr, 1);
  }
  Serial.println(GF_high, 8);
  Serial.println(NOS_high, 8);

  Serial.println(F("Calibration Done"));
}

void setup() {
  Serial.begin(115200);

  Ethernet.begin(mac);
  delay(1000); //give the ethernet a second to initialize
  // put your setup code here, to run once:
  CT.configureAD5934(15, 8 * pow(10, 3), 4, 2);     // number of settling times ,start frequency (Hz),frequency increment (Hz), number of increments
  delay(5);
  calibrateCN0349('H');

 if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }


  ads.setGain(GAIN_ONE);  
if (!ads.begin(0x49)) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }

}

void loop() {
 
MQTT_connect();
 

  float YL, YH, NH, NL, GF_low, NOS_low, GF_high, NOS_high = 0;
  float Y_cell, T_cell, YT_cell, T_imp, imp = -1;
  //Just please make sure calibrateCN0349 is called at least once in the programs lifetime before the following line!
  EEPROM_readAnything(GF_high_addr, GF_high);  //save values //GF_high = same as GF_rtd
  EEPROM_readAnything(NOS_high_addr, NOS_high);
  uint8_t CT_error = CT.measure(GF_high, GF_high, NOS_high, 'H', &T_imp, &imp, &Y_cell, &T_cell);
  Serial.print(F("Admittance:\t\t"));
  Serial.println(Y_cell, 3);

 float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;



  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
 // loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
//  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
 // Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  volt.publish(loadvoltage);
  curr.publish(current_mA);
int16_t adc0;
float volts0;
  adc0 = ads.readADC_SingleEnded(0);
    volts0 = ads.computeVolts(adc0);
     float pH1 = 4.462*volts0-0.822;
    Serial.print("AIN0: "); Serial.print(adc0); Serial.print("  "); Serial.print(volts0); Serial.println("V"); Serial.print(pH1); Serial.println("pH");\
  ph.publish(pH1);
  cond.publish(Y_cell);


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