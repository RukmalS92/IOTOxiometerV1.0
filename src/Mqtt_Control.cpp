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

void MqttControl::MqttConnect(){
    if(!client.connected())
    {
        client.connect(clientName);
    }
    while (!client.connected())
    {
        client.connect(clientName);
        Serial.print("Waiting MQTT Server...");
        delay(100);
    }
    
    if(client.connected()){
        Serial.println("Connected MQTT Server...");
    }
}

void MqttControl::MqttInit(){
    //wifi
    if(WiFi.status()!=WL_CONNECTED){
        WiFi.begin(wifiSSID, wifiPASS);
    }
    while (WiFi.status()!=WL_CONNECTED)
    {
        Serial.print(".") ;
    }
    if(WiFi.status() == WL_CONNECTED){
        Serial.println("Connected");
        Serial.print("locl IP ::");
        Serial.println(WiFi.localIP());
    }

    //Mqtt
    client.setServer(mqtt_server, _Port);
    MqttControl::MqttConnect();


}

void MqttControl::MqttPublish(const char* Topic,const char* wrstring ){
    client.publish(Topic, wrstring);
    client.loop();
}





