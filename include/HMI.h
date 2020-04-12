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
        //General Vars
        bool ShowBPFlag;
        bool ShowSpO2BPM;
        bool MainResetFlag;
        bool CyclicPaientDataUpdatingFlag;
        bool CyclicSystemDataUpdatingFlag;
        bool DeviceBusUpdating;

        HMI(MqttControl* mqttpointer);
        ~HMI();

        //inner methods
        void AllDevicesInit();
        void SetPatientData(int parameter, int value);
       
        void Update();


    private:
        long I2CBusWriteTimeStamp;
        long CyclicPatientDataUpdateTimeStamp;
        long CyclicSystemDataUpdateTimeStamp;
        
        bool BusBusyFlag;

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
        void UpdateCyclicPatientData();
        void UpdateCyclicSystemData();
        void DisplayUpdate();
        void HardReset();
        


    protected:
};

#endif