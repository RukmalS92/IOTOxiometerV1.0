#ifndef _OLED_H
#define _OLED_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <main.h>

//connection timeoutcheck
#define _CON_TIMEOUT 3000

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
        void SetPatientParameters(int parameter, int value);

        //Updaters
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

        bool GetDisplay1UpdateBusy();
        bool GetDisplay2UpdateBusy();
        /*******************************************************/
        //Getters
        bool GetDisplay1ERROR();
        bool GetDisplay2ERROR();
        
    private:

        enumPatientParam patientparameters;

        Adafruit_SSD1306 display1;
        Adafruit_SSD1306 display2;
        long oledtimestamp;

        long connectiontimeout;

        bool updaterequestdisplay1;
        bool updaterequestdisplay2;

        bool display1updatecomplete;
        bool display2updatecomplete;

        //Internal VAraiables
        int SpO2;
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
        bool display1connection;
        bool display2connection;

        void Display1StartupPage();
        void Display2StartupPage();

    
    protected:
};

#endif