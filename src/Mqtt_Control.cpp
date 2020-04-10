#include <Arduino.h>
#include <Mqtt_Control.h>
#include <WiFi.h>
#include <PubSubClient.h>

MqttControl::MqttControl(){

}

MqttControl::MqttControl(IPAddress serverIP, const char* wifissid, const char* wifipass,  const char* clientname, int port, const char* user, const char* pass){
    wifiSSID = wifissid;
    wifiPASS = wifipass;
    mqtt_server = serverIP;
    clientName = clientname;
    user = user;
    pass = pass;
    _Port = port;
    client.setClient(espClient);
}

MqttControl::~MqttControl(){
    
}

void MqttControl::SetupLocalIPV4(IPAddress add, IPAddress gateway, IPAddress sub){
    if(WiFi.config(add, gateway, sub)){
        Serial.println("Wifi settings config done...");
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
            connectiontimeouttime = millis();
        }
        while (WiFi.status()!=WL_CONNECTED)
        {
            Serial.print(".") ;
            if((millis() - connectiontimeouttime) > TIMEOUT_WIFI){
                Serial.println("Could Not Connect to Host");
                break;
            }
        }
        if(WiFi.status() == WL_CONNECTED){
            Serial.println("Connected");
            Serial.print("locl IP ::");
            Serial.println(WiFi.localIP());
            break;
        }
    }
    //Mqtt
    client.setServer(mqtt_server, _Port);
    MqttControl::MqttConnect();

}

void MqttControl::MqttConnect(){
    for(int i = 0; i < TRYOUTS; i++){
        if(!client.connected())
        {
            client.connect(clientName);
            connectiontimeouttime = millis();
            Serial.println("Waiting MQTT Server...");
        }
        while (!client.connected())
        {
            Serial.print(".");
            if((millis() - connectiontimeouttime) > TIMEOUT_WIFI){
                Serial.println("Could Not Connect to Server");
                break;
            }
        }
        if(client.connected()){
            Serial.println("Connected MQTT Server...");
            break;
        }
    }
}


void MqttControl::MqttPublish(const char* Topic,const char* wrstring ){
    client.publish(Topic, wrstring);
    client.loop();
}

void MqttControl::Reconnect(){

}





