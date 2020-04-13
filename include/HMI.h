#ifndef _HMI_H
#define _HMI_H

#include <Arduino.h>
#include <main.h>
#include <Mqtt_Control.h>
#include <Push_Button.h>
#include <BuzzerSound.h>
#include <BatteryHealth.h>
#include <Led.h>

class HMI{
    public:
        HMI(MqttControl* mqttpointer);
        ~HMI();

        //inner methods
        void AllDevicesInit();
        void SetPatientData(enumPatientParam parameter, int value);
       
        void Update();


    private:
         //General Vars
        bool CyclicPaientDataUpdatingFlag;
        bool CyclicSystemDataUpdatingFlag;
        bool ManualSpO2UpdateFlag;
        bool ManaulBPUpdateFlag;
        bool BusBusyFlag;

        long I2CBusWriteTimeStamp;
        long CyclicPatientDataUpdateTimeStamp;
        long CyclicSystemDataUpdateTimeStamp;
        
        

        int batterycharge;
        int wifisignal;

        //class pointer for callback
        MqttControl* mqttcontrol;

        //others
        PushButton pushbutton;
        Buzzer buzzer;
        Led led;
        BatteryHealth batteryhealth;

        void CheckButtons();
        void cyclicupdaterequest();
        void UpdateCyclicPatientData();
        void UpdateCyclicSystemData();
        void UpdateImmediateSpO2Request();
        void UpdateImmediateBPRequest();
        void updateSpO2BPM();
        void updateBP();
        void DisplayUpdate();
        void HardReset();
        


    protected:
};

#endif