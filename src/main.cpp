#include <Arduino.h>
#include <ArduinoJson.h>
#include <SevSeg.h>
#include <Push_Button.h>
#include <main.h>
#include <Oled.h>
#include <Mqtt_Control.h>
#include <BatteryHealth.h>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#include <Controller.h>

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

StaticJsonDocument<200> doc;
char jsonbuffer[512];

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
PushButton button;
BatteryHealth battery;
Controller controller;

int MainTimeStamp = 0;

long timebuf = 0;
long timebuf2 = 0;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  mqttclient.SetupLocalIPV4(localIP, gateway, subnet);
  mqttclient.MqttInit();
  controller.AllDevicesInit();
  button.ButtonInit();
  battery.BatteryHealthInit();
  mqttclient.SetData(SPO2, 42);
  mqttclient.SetData(BPM, 10);

}

void loop() {
  
  
  button.UpdateButton();

  if(button.GetMainButtonPressedstate() == true){
      mqttclient.SetDocCallPublishRequest();
  }
  if(button.GetCtrl2ButtonPressedstate() == true){
      mqttclient.ClearDocCallPublishRequest();
  }

  battery.UpdateBatteryMonitoring();
  controller.ReadNonDeviceVars(battery.GetBatteryHealth(), 35);
  controller.Update();
  mqttclient.MqttUpdate();
  
}