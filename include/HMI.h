#ifndef _HMI_H
#define _HMI_H

#include <Arduino.h>
#include <main.h>
#include <Mqtt_Control.h>
#include <Push_Button.h>
#include <BuzzerSound.h>
#include <BatteryHealth.h>
#include <Led.h>
#include <definition.h>

class HMI{
    public:
        HMI(MqttControl* mqttpointer);
        ~HMI();

        //inner methods
        void AllDevicesInit();
        //normal setters
        void SetPatientData(enumPatientParam parameter, int value); //set data values for all param
        //Set the state of sequance
        void SetCyclicOxiometerUpdateSeq(enumOximeterStatus state); 
        void SetManualOxiometerUpdateSeq(enumOximeterStatus state); 
        //return current state of each oxiometer cycle weather current executing is
        //turnon, wait satable, get data, turn off 
        enumOximeterReturnState GetCyclicOxiometerState(); //return cyclic reading
        enumOximeterReturnState GetManualOxiometerState(); //return 4 states @ user states

        /*------------------------------Action Requests Setters---------------------------------*/
        //Set cyclic Update Request
        void SetCyclicOxiometerUpdateRequest(); 

        /*------------------------------Action Status Getter-----------------------------------*/
        //get the busy state of cyclic oximeter update
        bool GetCyclicOxiometerUpdateBusy(); 
        //Get the Busy Flag When Request Happen
        bool GetOxiometerManualUpdateStatus();

        //display update busy
        bool GetDisplayUpdateBusy(); 

        //Update
        void Update();

    private:
        //States for Oxiometer Cycle
        enumOximeterStatus cyclicoxiometerstatus;
        enumOximeterStatus Lastcyclicoxiometerstatus;
        enumOximeterStatus manualoxiometerstatus;
        enumOximeterStatus Lastmanualoxiometerstatus;

        //return Indication State
        enumOximeterReturnState cyclicoxiometerStateReturn;
        enumOximeterReturnState manualoxiometerStateReturn;


        //HMI State
        HMISystemState hmiexecstate;

        //Action for each process
        Action actionoxiometercyclic;
        Action actionoxiometermanual;

        //internal flags for requesting
        bool CyclicOxiometerUpdateRequestFlag;
        bool ManualOxiometerUpdateRequestFlag;
        bool CyclicSystemDataUpdateRequestFlag;

        //methods
        void CheckButtons();
        void UpdateSequance();
        void UpdateImmediateOxiometerRequest();
        void UpdateCyclicOxiometerData();
        void UpdateCyclicSystemData();
        void ManualupdateOxiometer();
        void Actions();
        void Transitions();

        
        //class pointer for callback
        MqttControl* mqttcontrol;

        //others
        PushButton pushbutton;
        Buzzer buzzer;
        Led led;
        BatteryHealth batteryhealth;

        
        void cyclicupdaterequest();
        
        
        void UpdateImmediateBPRequest();
        
        void updateBP();
        void DisplayUpdate();
        void HardReset();
        void HandleErrors();
        
    protected:
};

#endif