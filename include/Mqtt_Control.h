#ifndef _MQTT_CONTROL_H
#define _MQTT_CONTROL_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Mqtt_Control.h>
#include <main.h>

/*Wifi Time out Time*/
#define TIMEOUT_WIFI 1000
#define TRYOUTS 3
#define PUBLISH_DELAY_TIME 3500
#define jsonpatientdatabufferchars 128
#define jsondoccalldatabufferchars 50

class MqttControl{

    public:
        int WifiStrength;
        
        MqttControl();
        MqttControl(IPAddress serverIP, const char* wifissid, const char* wifipass, 
                    const char* clientname, int port,const char* user, const char* pass);
        
        ~MqttControl();
        void SetupLocalIPV4(IPAddress add, IPAddress gateway, IPAddress sub);
        void MqttInit();
        void SetDataPublishRequest();
        void SetDocCallPublishRequest();
        void ClearDocCallPublishRequest();
        void SetData(int parameter, int value);
        void MqttUpdate();
        
    private:
        bool wifireconnect;
        bool serverreconnect;
        bool publishdatarequest;
        bool publishdocrequest;
        bool publishdocrequestclear;
        
        enumPatientParam patientdata;
        enumMqttStatus mqttstatu;

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
        void GetWifiStrength();
   
    protected:

};

#endif