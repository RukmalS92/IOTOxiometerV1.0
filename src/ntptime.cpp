#include <ntptime.h>
#include <Arduino.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <main.h>

WiFiUDP ntpUDP;

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, "0.asia.pool.ntp.org", 19800, 5000);

ntptime::ntptime(/* args */)
{
}

ntptime::~ntptime()
{
}

void ntptime::Init(){
    ntpupdatedone=false;
    if(WiFi.status() == WL_CONNECTED){
        timeClient.begin();
        this->ntpupdatedone = timeClient.forceUpdate();
    }    
}

void ntptime::ConvertTime(){
    this->formattedtime = timeClient.getFormattedTime();
    this->time = this->formattedtime.substring(0, (this->formattedtime.length() - 3));
    #ifdef USE_SERIAL_MONITOR
        //Serial.print("NTP Time : "); Serial.println(this->time);
    #endif
    
}

const char* ntptime::GetTime(){
    const char* p = this->time.c_str();
    return p;
}

void ntptime::update(){
    if(WiFi.status() == WL_CONNECTED){
        if(this->ntpupdatedone == false){
            timeClient.begin();
            this->ntpupdatedone = timeClient.forceUpdate();
        }
    }
    this->ConvertTime();
}
