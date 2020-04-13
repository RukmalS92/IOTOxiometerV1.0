#include <Arduino.h>
#include <SevSeg.h>
#include <main.h>
#include <Oled.h>
#include <Mqtt_Control.h>
#include <definition.h>
#include <Push_Button.h>
#include <HMI.h>

IPAddress localIP(192,168,1,22);
IPAddress serverIP(34,71,47,61);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

const char* wifissid = "RUK-WIFI";
const char* wifipass = "madushan2014Sen";
const char* cl = "1";
const char* user = "admin@123";
const char* pass = "admin@123";

MqttControl mqttclient(serverIP, wifissid, wifipass, cl, 1991, user, pass);
HMI hmi(&mqttclient);

int MainTimeStamp = 0;

long timebuf = 0;
long timebuf2 = 0;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  mqttclient.SetupLocalIPV4(localIP, gateway, subnet);
  mqttclient.MqttInit();
  hmi.AllDevicesInit();
  
  //this the only method to add param
  //check main.h for enum
  hmi.SetPatientData(SPO2, 82);
  hmi.SetPatientData(BPM, 74);
}

enumSpO2ReturnState nextstate;

void loop() {
  /*Here dummy for cyclic data update every 6000ms*/
  //this is just dummy you can use your own implementation for seq "enumSpO2ReturnState" is just for imeplementing seq
  //but you need to check these flags
  if(millis()-timebuf2 > 1000){
    if(hmi.GetCyclicOxiometerUpdateBusy() != true){
    /*Oxiometer Turn_on*/
    hmi.SetCyclicOxiometerUpdateRequest();
    hmi.SetCyclicOxiometerUpdateSeq(TURN_ON);
    nextstate = TURN_ON_DONE; 
    timebuf2 = millis();
    Serial.println("turn on done");
    }
    else if(hmi.GetCyclicOxiometerUpdateBusy() == true){
      switch (nextstate)
      {
      case TURN_ON_DONE:
        if(hmi.GetDisplayUpdateBusy() != true){
          hmi.SetCyclicOxiometerUpdateSeq(WAIT_STABLE);
          nextstate = STABLE_DONE;
          timebuf2 = millis();
          Serial.println("stabel done");
        }
        break;
      case STABLE_DONE:
        if(hmi.GetDisplayUpdateBusy() != true){
          hmi.SetPatientData(SPO2, 56);
          hmi.SetPatientData(BPM, 56);
          hmi.SetCyclicOxiometerUpdateSeq(GET_DATA);
          nextstate = GET_DATA_DONE;
          timebuf2 = millis();
          Serial.println("get data done");
        }
        break;
      case GET_DATA_DONE:
        if(hmi.GetDisplayUpdateBusy() != true){
          hmi.SetCyclicOxiometerUpdateSeq(TURN_OFF);
          nextstate = TURN_OFF_DONE;
          timebuf2 = millis();
          Serial.println("turn off done");
        }
      default:
        break;
      }
    }
  }
  
  

  /*Here dummy request for puhblish request every 6000ms*/
  if(millis() - timebuf > 6000){
    mqttclient.SetDataPublishRequest();
    timebuf = millis();
  }

  hmi.Update();
  mqttclient.MqttUpdate();
  
}