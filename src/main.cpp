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

bool internalflag=false;

long timebuf = 0;
long timebuf2 = 0;
long timebuf3 = 0;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  mqttclient.SetupLocalIPV4(localIP, gateway, subnet, IPAddress(8,8,8,8));
  mqttclient.MqttInit();
  hmi.AllDevicesInit();
  
  //this the only method to add param
  //check main.h for enum
  hmi.SetPatientData(SPO2, 82);
  hmi.SetPatientData(BPM, 74);
}

enumOximeterReturnState nextstate;
enumOximeterReturnState nextstate2;

void loop() {
  /*Here dummy for cyclic data update every 6000ms*/
  //this is just dummy you can use your own implementation for seq "enumSpO2ReturnState" is just for imeplementing seq
  //but you need to check these flags
  if(millis()-timebuf2 > 1000){
    if(hmi.GetCyclicOxiometerUpdateBusy() != true){
      /*Oxiometer Turn_on*/
      hmi.SetCyclicOxiometerUpdateRequest();
      
    }
    else if(hmi.GetCyclicOxiometerUpdateBusy() == true){
      switch (nextstate2)
      {
        case DEFAULT_STATE_RET:
          //Serial.println("0000000000000000000000000000");
          hmi.SetCyclicOxiometerUpdateSeq(TURN_ON);
          if(hmi.GetCyclicOxiometerState() == TURN_ON_RET){
              nextstate2 = TURN_ON_RET; 
              Serial.println("turn on done");
          }
           timebuf2 = millis();
          
          break;
        case TURN_ON_RET:
          //Serial.println("----------------------------");
          if(hmi.GetDisplayUpdateBusy() != true){
            hmi.SetCyclicOxiometerUpdateSeq(WAIT_STABLE);
            if(hmi.GetCyclicOxiometerState() == STABLE_RET){
              nextstate2 = STABLE_RET;
            }
            
            timebuf2 = millis();
            //Serial.println("stabel done");
          }
          break;
        case STABLE_RET:
          //Serial.println("+++++++++++++++++++++++++++");
          if(hmi.GetDisplayUpdateBusy() != true){
            hmi.SetPatientData(SPO2, 56);
            hmi.SetPatientData(BPM, 56);
            hmi.SetCyclicOxiometerUpdateSeq(GET_DATA);
            if(hmi.GetCyclicOxiometerState() == GET_DATA_RET){
              nextstate2 = GET_DATA_RET;
              //Serial.println("get data done");
            }
            
            timebuf2 = millis();
            
          }
          break;
        case GET_DATA_RET:
            //Serial.println("********************************");
              if(hmi.GetDisplayUpdateBusy() != true){
                hmi.SetCyclicOxiometerUpdateSeq(TURN_OFF);
                if(hmi.GetCyclicOxiometerState() == DEFAULT_STATE_RET){
                  nextstate2 = DEFAULT_STATE_RET;
                  //Serial.println("turn off done");
                }
                timebuf2 = millis();
              }
          break;
        default:
          break;
      }
    }
  }




  if(millis()-timebuf3 > 300){
    if(hmi.GetOxiometerManualUpdateStatus() == true && internalflag == false){
      /*Oxiometer Turn_on*/
      internalflag = true;
      //Serial.println("Maual turn on done");
    }
    else if(hmi.GetOxiometerManualUpdateStatus() == true && internalflag == true){
      switch (nextstate)
      {
        case DEFAULT_STATE_RET:
          //Serial.println("0000000000000000000000000000");
          hmi.SetManualOxiometerUpdateSeq(TURN_ON);
          if(hmi.GetManualOxiometerState() == TURN_ON_RET){
              nextstate = TURN_ON_RET; 
          }
           timebuf3= millis();
          //Serial.println("turn on done");
          break;
        case TURN_ON_RET:
          //Serial.println("----------------------------");
          if(hmi.GetDisplayUpdateBusy() != true){
            hmi.SetManualOxiometerUpdateSeq(WAIT_STABLE);
            if(hmi.GetManualOxiometerState() == STABLE_RET){
              nextstate = STABLE_RET; 
            }
            
            timebuf3 = millis();
            //Serial.println("Maual stabel done");
          }
          break;
        case STABLE_RET:
          //Serial.println("+++++++++++++++++++++++++++");
          if(hmi.GetDisplayUpdateBusy() != true){
            hmi.SetPatientData(SPO2, 53);
            hmi.SetPatientData(BPM, 23);
            hmi.SetManualOxiometerUpdateSeq(GET_DATA);
            if(hmi.GetManualOxiometerState() == GET_DATA_RET){
              nextstate = GET_DATA_RET; 
            }
            
            timebuf3 = millis();
            //Serial.println("Maual get data done");
          }
          break;
        case GET_DATA_RET:
        ///Serial.println("********************************");
          if(hmi.GetDisplayUpdateBusy() != true){
            hmi.SetManualOxiometerUpdateSeq(TURN_OFF);
            if(hmi.GetManualOxiometerState() == DEFAULT_STATE_RET){
              nextstate = DEFAULT_STATE_RET; 
            }
            timebuf3 = millis();
            //Serial.println("Maual turn off done");
            internalflag = false;
          }
          break;
        default:
          break;
      }
    }
  }
  

  
  if(millis() - timebuf > 6000){
    hmi.SetCyclicSystemDataUpdateRequest();
    mqttclient.SetDataPublishRequest();
    timebuf = millis();
  }

  hmi.Update();
  //mqttclient.MqttUpdate();
  
}