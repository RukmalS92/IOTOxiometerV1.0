#ifndef _MQTT_CONTROL_H
#define _MQTT_CONTROL_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Mqtt_Control.h>

/*Wifi Time out Time*/
#define TIMEOUT_WIFI 1000
#define TRYOUTS 3
#define PUBLISH_DELAY_TIME 3500

class MqttControl{

    public:
        int WifiStrength;
        
        MqttControl();
        MqttControl(IPAddress serverIP, const char* wifissid, const char* wifipass, 
                    const char* clientname, int port,const char* user, const char* pass);
        
        ~MqttControl();
        void GetWifiStrength();
        void SetupLocalIPV4(IPAddress add, IPAddress gateway, IPAddress sub);
        void MqttInit();
        void MqttConnect();
        void setTpoic(const char* topic);
        void MqttUpdate();
        void Reconnect();

    private:
        const char* wifiSSID;
        const char* wifiPASS;
        const char* user;
        const char* pass;
        const char* Topic;

        /*passing value for uploading*/
        int spo2;
        int bpm;
        int bpsystolic;
        int bpdiastolic;

        long connectiontimeouttime;
        long publishtime;

        IPAddress mqtt_server;
        IPAddress localIP;
        const char* clientName;
        int _Port;

        StaticJsonDocument<200> doc;
        char jsonbuffer[512];

        WiFiClient espClient;
        PubSubClient client;

        void MqttPublish();

    
    protected:

};

#endif