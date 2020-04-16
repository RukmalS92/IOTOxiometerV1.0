#ifndef _NTPTIME_H
#define _NTPTIME_H
#include <Arduino.h>
#include <NTPClient.h>
class ntptime
{
private:
    String time;
    String formattedtime;
    void ConvertTime();

    bool ntpupdatedone;
   
public:
    ntptime(/* args */);
    ~ntptime();
    void Init();
    void update();
    
    const char* GetTime();

};




#endif