#include <string.h>
#include <iostream>
#include <Arduino.h>
#include <Mqtt_Control.h>
#include <main.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <definition.h>

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
    MqttControl::patientTopic = "sensors";
    MqttControl::docTopic = "alarm";
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

int MqttControl::GetWifiStrength(){
    int x = map(abs(WiFi.RSSI()), 0, 60, 0, 100);
    MqttControl::WifiStrength = x;
    return MqttControl::WifiStrength;
}


void MqttControl::MqttInit(){
    wifireconnect=false;
    serverreconnect=false;
    publishdatarequest=false;
    publishdocrequest=false;
    publishdocrequestclear=false;

    spo2=0;
    bpm=0;
    bpsystolic=0;
    bpdiastolic=0;

    connectiontimeouttime=0;
    publishtime=0;
    reconnecttime=0;
    //wifi
    for(int i = 0; i < TRYOUTS; i++){
        if(WiFi.status()!=WL_CONNECTED){
            WiFi.begin(MqttControl::wifiSSID, MqttControl::wifiPASS);
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
    MqttControl::client.setServer(MqttControl::mqtt_server, MqttControl::_Port);
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


void MqttControl::MqttPublish(){
    /*{"devId": "1","spo2": "1.2","bpm": "62","sypre": "1.2","dipre": "1.2"}*/
    /*sensors*/
    if((millis() - MqttControl::publishtime) > PUBLISH_DELAY_TIME && MqttControl::publishdatarequest == true){
        memset(MqttControl::jsonbuffer, 0, jsonpatientdatabufferchars);
        MqttControl::doc["devId"] = MqttControl::clientName;
        MqttControl::doc["spo2"] = String(MqttControl::spo2);
        MqttControl::doc["bpm"] = String(MqttControl::bpm);
        MqttControl::doc["sypre"] = String(MqttControl::bpsystolic);
        MqttControl::doc["dipre"] = String(MqttControl::bpdiastolic);
       
        size_t buffersize = serializeJson(MqttControl::doc, MqttControl::jsonbuffer);
        if(MqttControl::client.connected())
        {
            client.publish(MqttControl::patientTopic, MqttControl::jsonbuffer, buffersize);
            MqttControl::publishdatarequest = false;
            #ifdef USE_SERIAL_MONITOR
                Serial.println("MQTT Publishing Patient data...");
            #endif

        }
         MqttControl::publishtime = millis();
           
    }
    else if ((millis() - MqttControl::publishtime) > PUBLISH_DELAY_TIME && MqttControl::publishdocrequest == true)
    {
        memset(MqttControl::jsondoccallbuffer, 0, jsondoccalldatabufferchars);
        MqttControl::calldoc["devId"] = MqttControl::clientName;
        MqttControl::calldoc["state"] = "true";
        size_t buffersize = serializeJson(MqttControl::calldoc, MqttControl::jsondoccallbuffer);
        if(MqttControl::client.connected())
        {
            client.publish(MqttControl::docTopic, MqttControl::jsondoccallbuffer, buffersize);
            MqttControl::publishdocrequest = false;
            #ifdef USE_SERIAL_MONITOR
                Serial.println("MQTT Publishing docRequest...");
            #endif

        }
         MqttControl::publishtime = millis();
    }
    else if ((millis() - MqttControl::publishtime) > PUBLISH_DELAY_TIME && MqttControl::publishdocrequestclear == true)
    {
        memset(MqttControl::jsondoccallbuffer, 0, jsondoccalldatabufferchars);
        MqttControl::calldoc["devId"] = MqttControl::clientName;
        MqttControl::calldoc["state"] = "false";
        size_t buffersize = serializeJson(MqttControl::calldoc, MqttControl::jsondoccallbuffer);
        if(MqttControl::client.connected())
        {
            client.publish(MqttControl::docTopic, MqttControl::jsondoccallbuffer, buffersize);
            MqttControl::publishdocrequestclear = false;
            #ifdef USE_SERIAL_MONITOR
                Serial.println("MQTT Publishing docRequestClear...");
            #endif

        }
         MqttControl::publishtime = millis();
    }
    
    
}

void MqttControl::Reconnect(){
    if(!MqttControl::client.connected()){
        if(WiFi.status()!=WL_CONNECTED && MqttControl::wifireconnect != true){
            WiFi.begin(MqttControl::wifiSSID, MqttControl::wifiPASS);
            MqttControl::wifireconnect = true;
            MqttControl::reconnecttime = millis();
            #ifdef USE_SERIAL_MONITOR
                Serial.println("WIFI ReConnecting... ");
            #endif
        }
        else if (WiFi.status()!=WL_CONNECTED && MqttControl::wifireconnect == true  
                                    && (millis() - MqttControl::reconnecttime) < TIMEOUT_WIFI)
        {
            #ifdef USE_SERIAL_MONITOR
                Serial.print(".");
            #endif
        }
        else if (WiFi.status()!=WL_CONNECTED && MqttControl::wifireconnect == true  
                                    && (millis() - MqttControl::reconnecttime) > TIMEOUT_WIFI)
        {
            MqttControl::wifireconnect = false;
        }
        else if(WiFi.status()==WL_CONNECTED){
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Connected");
                Serial.print("locl IP ::");
                Serial.println(WiFi.localIP());
            #endif
            MqttControl::wifireconnect = false;
            if(!MqttControl::client.connected() &&  MqttControl::serverreconnect != true){
                MqttControl::client.connect(MqttControl::clientName, MqttControl::user, MqttControl::pass);
                MqttControl::serverreconnect = true;
                MqttControl::reconnecttime = millis();
                #ifdef USE_SERIAL_MONITOR
                    Serial.println("Server ReConnecting... ");
                #endif
            }
            else if (!MqttControl::client.connected() &&  MqttControl::serverreconnect == true 
                                        && (millis() - MqttControl::reconnecttime) < TIMEOUT_WIFI)
            {
                #ifdef USE_SERIAL_MONITOR
                    Serial.print(".");
                #endif
            }
            else if (!MqttControl::client.connected() &&  MqttControl::serverreconnect == true 
                                        && (millis() - MqttControl::reconnecttime) > TIMEOUT_WIFI)
            {
                MqttControl::serverreconnect = false;
            }
            else if (MqttControl::client.connected())
            {
                #ifdef USE_SERIAL_MONITOR
                    Serial.print("Server Connected....");
                #endif
                MqttControl::serverreconnect = false;
            }    
        }

    }
    
}

void MqttControl::MqttUpdate(){
    MqttControl::Reconnect();
    MqttControl::MqttPublish();
    MqttControl::client.loop();
}

void MqttControl::SetDataPublishRequest(){
    MqttControl::publishdatarequest = true;
}
void MqttControl::SetDocCallPublishRequest(){
    MqttControl::publishdocrequest = true;
}
void MqttControl::ClearDocCallPublishRequest(){
    MqttControl::publishdocrequestclear = true;
}

void MqttControl::SetData(enumPatientParam param, int val){
    switch (param)
    {
    case SPO2:
        MqttControl::spo2 = val;
        break;
    case BPM:
        MqttControl::bpm = val;
        break;
    case BPsys:
        MqttControl::bpsystolic = val;
        break;
    case BPdias:
        MqttControl::bpdiastolic = val;
        break;
    
    default:
        break;
    }
}






