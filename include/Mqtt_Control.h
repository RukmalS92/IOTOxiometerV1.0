#ifndef _MQTT_CONTROL_H
#define _MQTT_CONTROL_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

class MqttControl{
    public:
        int WifiStrength;

        MqttControl();
        MqttControl(IPAddress serverIP, const char* wifissid, const char* wifipass, const char* clientname, int port,const char* user, const char* pass);
        
        ~MqttControl();
        void GetWifiStrength();
        void SetupLocalIPV4(IPAddress add, IPAddress gateway, IPAddress sub);
        void MqttInit();
        void MqttConnect();
        void MqttPublish(const char* Topic,const char* wrstring);
    private:
        const char* wifiSSID;
        const char* wifiPASS;
        const char* user;
        const char* pass;
        IPAddress mqtt_server;
        IPAddress localIP;
        const char* clientName;
        int _Port;

        

        WiFiClient espClient;
        PubSubClient client;
       
    protected:

};

#endif