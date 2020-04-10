#include <string.h>
#include <iostream>
#include <Arduino.h>
#include <Mqtt_Control.h>
#include <main.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

using namespace std;

MqttControl::MqttControl(){

}

MqttControl::MqttControl(IPAddress serverIP, const char* wifissid, const char* wifipass,  
                            const char* clientname, int port, const char* user, const char* pass){
    MqttControl::wifiSSID = wifissid;
    MqttControl::wifiPASS = wifipass;
    MqttControl::mqtt_server = serverIP;
    MqttControl::clientName = clientname;
    MqttControl::user = user;
    MqttControl::pass = pass;
    MqttControl::_Port = port;
    MqttControl::client.setClient(espClient);
}

MqttControl::~MqttControl(){
    
}

void MqttControl::SetupLocalIPV4(IPAddress add, IPAddress gateway, IPAddress sub){
    if(WiFi.config(add, gateway, sub)){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Wifi settings config done...");
        #endif
    }
}

void MqttControl::GetWifiStrength(){
    int x = map(abs(WiFi.RSSI()), 0, 60, 0, 100);
    MqttControl::WifiStrength = x;
}


void MqttControl::MqttInit(){
    //wifi
    for(int i = 0; i < TRYOUTS; i++){
        if(WiFi.status()!=WL_CONNECTED){
            WiFi.begin(wifiSSID, wifiPASS);
            MqttControl::connectiontimeouttime = millis();
        }
        while (WiFi.status()!=WL_CONNECTED)
        {
            Serial.print(".") ;
            if((millis() - MqttControl::connectiontimeouttime) > TIMEOUT_WIFI){
                #ifdef USE_SERIAL_MONITOR
                    Serial.println("Could Not Connect to Host");
                #endif
                break;
            }
        }
        if(WiFi.status() == WL_CONNECTED){
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Connected");
                Serial.print("locl IP ::");
                Serial.println(WiFi.localIP());
            #endif
            break;
        }
    }
    //Mqtt
    MqttControl::client.setServer(mqtt_server, _Port);
    MqttControl::MqttConnect();

}

void MqttControl::MqttConnect(){
    for(int i = 0; i < TRYOUTS; i++){
        if(!MqttControl::client.connected())
        {
            MqttControl::client.connect(MqttControl::clientName, MqttControl::user, MqttControl::pass);
            MqttControl::connectiontimeouttime = millis();
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Waiting MQTT Server...");
            #endif
        }
        while (!MqttControl::client.connected())
        {
            Serial.print(".");
            if((millis() - MqttControl::connectiontimeouttime) > TIMEOUT_WIFI){
                #ifdef USE_SERIAL_MONITOR
                    Serial.println("Could Not Connect to Server");
                #endif
                break;
            }
        }
        if(MqttControl::client.connected()){
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Connected MQTT Server...");
            #endif
            break;
        }
    }
}

void MqttControl::setTpoic(const char* topic){
    MqttControl::Topic = topic;
}

void MqttControl::MqttPublish(){
    /*{"devId": "1","spo2": "1.2","bpm": "62","sypre": "1.2","dipre": "1.2"}*/
    /*sensors*/
    if((millis() - MqttControl::publishtime) > PUBLISH_DELAY_TIME){
        #ifdef USE_SERIAL_MONITOR
                Serial.println("MQTT Publishing...");
        #endif
        /*
        MqttControl::doc["devid"] = MqttControl::clientName;
        MqttControl::doc["spo2"] = MqttControl::spo2;
        MqttControl::doc["bpm"] = MqttControl::bpm;
        MqttControl::doc["sypre"] = MqttControl::bpsystolic;
        MqttControl::doc["dipre"] = MqttControl::bpdiastolic;*/
        MqttControl::doc["devId"] = "1";
        MqttControl::doc["spo2"] = "99";
        MqttControl::doc["bpm"] = "74";
        MqttControl::doc["sypre"] = "90";
        MqttControl::doc["dipre"] = "85";

        serializeJson(MqttControl::doc, MqttControl::jsonbuffer);
        if(MqttControl::client.connected())
        {
            client.publish(MqttControl::Topic, MqttControl::jsonbuffer);
        }
         MqttControl::publishtime = millis();
           
    }
}

void MqttControl::Reconnect(){

}

void MqttControl::MqttUpdate(){
    MqttControl::MqttPublish();
    MqttControl::client.loop();
}





