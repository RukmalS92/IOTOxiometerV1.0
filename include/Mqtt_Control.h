#ifndef _MQTT_CONTROL_H
#define _MQTT_CONTROL_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Mqtt_Control.h>

/*Wifi Time out Time*/
#define TIMEOUT_WIFI 1000
#define TRYOUTS 3

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
        void MqttPublish(const char* Topic,const char* wrstring);
        void MqttUpdate();
        void Reconnect();
    private:
        const char* wifiSSID;
        const char* wifiPASS;
        const char* user;
        const char* pass;

        long connectiontimeouttime;

        IPAddress mqtt_server;
        IPAddress localIP;
        const char* clientName;
        int _Port;

        WiFiClient espClient;
        PubSubClient client;

        
       
    protected:

};

#endif