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
        //normal setters
        void SetPatientData(enumPatientParam parameter, int value); //set data values for all param

        //sequance setters
        //Set cyclic Update Request
        void SetCyclicOxiometerUpdateRequest(); 
        //Set the state of sequance
        void SetCyclicOxiometerUpdateSeq(enumSpO2Status state); 


        //Sequance getters
        //get the busy state of cyclic spo2 update
        bool GetCyclicOxiometerUpdateBusy(); 


        //General getters
        //display update busy
        bool GetDisplayUpdateBusy(); 

        //Update
        void Update();

    private:
        //pass var to in
        enumSpO2Status spo2status;
        enumSpO2ReturnState spo2currentstate;
        //General Vars
        //Spo2BPM cyclic
        bool CyclicOxiometerUpdateRequestFlag;
        bool CyclicOxiometerUpdateBusyFlag;

        bool CyclicSystemDataUpdatingFlag;
        //spo2BPM @ request
        bool ManualOxiometerUpdateRequestFlag;
        bool ManualOxiometerUpdateBusyFlag;

        //BP @ request
        bool ManaulBPUpdateRequestFlag;
        bool ManaulBPUpdateBusyFlag;
        //bool BusBusyFlag;

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
        void UpdateCyclicOxiometerData();
        void UpdateCyclicSystemData();
        void UpdateImmediateOxiometerRequest();
        void UpdateImmediateBPRequest();
        void updateOxiometer();
        void updateBP();
        void DisplayUpdate();
        void HardReset();
        void HandleErrors();
        


    protected:
};

#endif