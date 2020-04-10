#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <Arduino.h>
class Controller{
    public:
        //General Vars
        bool ShowBPFlag;
        bool ShowSpO2BPM;
        bool MainResetFlag;
        bool CyclicPaientDataUpdatingFlag;
        bool CyclicSystemDataUpdatingFlag;
        bool DeviceBusUpdating;

        Controller();
        ~Controller();

        //inner methods
        void AllDevicesInit();
        void ReadNonDeviceVars(int batteryHealth, int wifistrength);
        void ReadSpo2();
        void ReadBPM();
        void ReadBloodPressure();
        void ReadBatteryHealth();

        void UpdateDisplay1atRequest();
        void UpdateDisplay2atRequest();
        void UpdateCyclicPatientData();
        void UpdateCyclicSystemData();
        void UpdateUserRequestFlags();

        
        void WriteDisplay1();
        void WriteDisplay2();

    private:
        long I2CBusWriteTimeStamp;
        long CyclicPatientDataUpdateTimeStamp;
        long CyclicSystemDataUpdateTimeStamp;
        
        bool BusBusyFlag;

        int BatteryHealth;
        int wifisignal;


    protected:
};

#endif