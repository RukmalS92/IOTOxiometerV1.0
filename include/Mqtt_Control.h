#ifndef _MQTT_CONTROL_H
#define _MQTT_CONTROL_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Mqtt_Control.h>
#include <main.h>
#include <definition.h>



class MqttControl{

    public:
        int WifiStrength;
        
        MqttControl();
        MqttControl(IPAddress serverIP, const char* wifissid, const char* wifipass, 
                    const char* clientname, int port,const char* user, const char* pass);
        
        ~MqttControl();
        void SetupLocalIPV4(IPAddress add, IPAddress gateway, IPAddress sub, IPAddress dns);
        void MqttInit();
        void SetDataPublishRequest();
        void SetDocCallPublishRequest();
        void ClearDocCallPublishRequest();
        void SetData(enumPatientParam param, int val);
        void MqttUpdate();
        int GetWifiStrength();
        
    private:
        bool wifireconnect;
        bool serverreconnect;
        bool publishdatarequest;
        bool publishdocrequest;
        bool publishdocrequestclear;
        
        const char* wifiSSID;
        const char* wifiPASS;
        const char* user;
        const char* pass;
        const char* patientTopic;
        const char* docTopic;

        /*passing value for uploading*/
        int spo2;
        int bpm;
        int bpsystolic;
        int bpdiastolic;

        long connectiontimeouttime;
        long publishtime;
        long reconnecttime;

        IPAddress mqtt_server;
        IPAddress localIP;
        const char* clientName;
        int _Port;

        StaticJsonDocument<128> doc;
        StaticJsonDocument<50> calldoc;
        char jsonbuffer[jsonpatientdatabufferchars];
        char jsondoccallbuffer[jsondoccalldatabufferchars];

        WiFiClient espClient;
        PubSubClient client;

        void MqttPublish();
        void Reconnect();
        void MqttConnect();
        
    protected:

};

#endif