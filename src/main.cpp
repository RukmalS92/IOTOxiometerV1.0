#include <Arduino.h>
#include <ArduinoJson.h>
#include <SevSeg.h>
#include <Push_Button.h>
#include <main.h>
#include <Oled.h>
#include <Mqtt_Control.h>
#include<BatteryHealth.h>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#include <Controller.h>

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

StaticJsonDocument<200> doc;
char jsonbuffer[512];

//Oled Constructor --INNER--
//Adafruit_SSD1306 disp1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//Adafruit_SSD1306 disp2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//Oled Display(disp1, disp2);


IPAddress localIP(192,168,1,22);
IPAddress serverIP(34,71,47,61);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

const char* wifissid = "RUK-WIFI";
const char* wifipass = "madushan2014Sen";
const char* cl = "cli";
const char* user = "mica";
const char* pass = "mica@autonomation";

MqttControl mqttclient(serverIP, wifissid, wifipass, cl, 1991, user, pass);
int MainTimeStamp = 0;

PushButton button;
BatteryHealth battery;
Controller controller;

long timebuf = 0;
long timebuf2 = 0;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  /*
  mqttclient.SetupLocalIPV4(localIP, gateway, subnet);
  mqttclient.MqttInit();
  doc["devid"] = "client1";
  doc["spo2"] = "1.2";
  doc["bpm"] = "62";
  doc["sypre"] = "1.2";
  doc["dipre"] = "1.6";
  
  serializeJson(doc, jsonbuffer);
  Serial.println(jsonbuffer);*/

  controller.AllDevicesInit();
  button.ButtonInit();
  battery.BatteryHealthInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  if(millis() - timebuf > 1000){
      mqttclient.MqttPublish("sensors", jsonbuffer);
      mqttclient.GetWifiStrength();
      timebuf = millis();
  }
  
  if(millis() - timebuf2 > 1000){
    controller.UpdateCyclicSystemData();
    timebuf2 = millis();
  }*/

  
  
  button.UpdateButton();
  battery.UpdateBatteryMonitoring();
  controller.UpdateUserRequestFlags();
  
}