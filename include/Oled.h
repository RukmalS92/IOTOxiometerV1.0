#ifndef _OLED_H
#define _OLED_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Oled{
    public:
        //Vars
        
        //Methods
        Oled();
        Oled(Adafruit_SSD1306 d1, Adafruit_SSD1306 d2);
        ~Oled();
        
        //Both the Displays are initialized.
        void DisplayInit();
        
        //Setup the Monitoing Display Scenne for updating sensors data.
        void Display1MonitorSceneSetup();
        void Display2MonitorSceneSetup();

        //Setters
        void SetWifiSignalDisplay(int signal);
        void SetBatteryHealthDisplay(int batteryvoltagel);
        void SetSPO2Display(int SPO2value);
        void SetBPMDisplay(int HeartRate);
        void SetBPMBitMap();
        void SetBPressureDisplay(int SystolicP, int DiastolicP);
        
        //Getters
        bool GetDisplay1ERROR();
        bool GetDisplay2ERROR();

        void UpdateDisplay1();
        void UpdateDisplay1(int *TimeStamp);
        void UpdateDisplay2();
        void UpdateDisplay2(int *TimeStamp);
   
    private:
        Adafruit_SSD1306 display1;
        Adafruit_SSD1306 display2;
        long Oled1TimeStamp;
        long Oled2TimeStamp;

        bool updaterequestdisplay1;
        bool updaterequestdisplay2;

        //value change ID
        int LastSPO2;
        int LastHeartRate;
        int LastPressureSystolic;
        int LastPressureDiastolic;
        int LastBatteryVoltage;
        int LastWifisignal;

        //Error Handling for Display Communication initlization
        bool Display1ConnectionError;
        bool Display2ConnectionError;

        void Display1StartupPage();
        void Display2StartupPage();

    
    protected:
};

#endif