#ifndef _OLED_H
#define _OLED_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


/*
    Display1 ----> BP, Wifi
    Display2 ----> SPO2, BPM, BatteryHealth
*/

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
        /******************************************************/
        //Setters
        void SetSPO2(int* value);
        void SetBPM(int* value);
        void SetBPressure(int* Sys, int* dias);
        void SetBatteryVoltage(int* batvalue);
        void SetWifiSignal(int* wifisignal);

    
        //Updaters
        void UpdatePatientDataDisplay1(); //updates BP
        void UpdatePatientDataDisplay2(); //updates spo2, BPM
        void UpdateSystemDataDisplay1(); //updates wifi
        void UpdateSystemDataDisplay2(); //updates battery
        /*******************************************************/
        //Getters
        bool GetDisplay1ERROR();
        bool GetDisplay2ERROR();
        
    private:
        Adafruit_SSD1306 display1;
        Adafruit_SSD1306 display2;
        long Oled1TimeStamp;
        long Oled2TimeStamp;

        bool updaterequestdisplay1;
        bool updaterequestdisplay2;

        //Internal VAraiables
        int SPO2;
        int HeartRate;
        int PressureSystolic;
        int PressureDiastolic;
        int BatteryVoltage;
        int WifiSignal;
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

        void ProcessWifiSignalDisplay();
        void ProcessBatteryHealthDisplay();
        void ProcessSetSPO2Display();
        void ProcessSetBPMDisplay();
        void ProcessSetBPMBitMap();
        void ProcessSetBPressureDisplay();

        void updatedisplay1();
        void updatedisplay1(int *TimeStamp);
        void updatedisplay2();
        void updatedisplay2(int *TimeStamp);

    protected:
};

#endif