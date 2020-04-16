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
#include <ntptime.h>

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
        enumOximeterReturnState GetCyclicOxiometerState(); //get the current state of oxi cycle @ cyclic
        enumOximeterReturnState GetManualOxiometerState(); //get the current state of oxi cycle @ manual request
        //return current state of each BP cycle weather current executing is
        //device ready, pumping stage etc.... 
        BPReturnStates GetCyclicBPState(); //get the current state of the BP cycle @ cyclic BP measure
        /*------------------------------Action Requests Setters---------------------------------*/
        //Set cyclic Update Request
        void SetCyclicOxiometerUpdateRequest(); 
        void SetCyclicSystemDataUpdateRequest();
        void SetCyclicBPUpdateRequest();

        /*------------------------------Action Status Getter-----------------------------------*/
        //get the busy state of cyclic oximeter update
        bool GetCyclicOxiometerUpdateBusy(); 
        //Get the Busy Flag When Request Happen ---> this will used as a callback method to MAIN get the reqtest
        //to run the manual reading methods of oxioemeter
        bool GetOxiometerManualUpdateStatus();
        //Get the Busy Flag when running the BP cycle - CYCLIC
        bool GetCyclicBPUpdateBusy();


        /*-------------------------Display Status Returns------------------------*/
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

        //BP cycle
        BPStates CyclicBPStatus;
        BPStates LastCyclicBPStatus;
        BPStates ManualBPStatus;
        BPStates LastManualBPStatus;
        
        //return Indication State
        enumOximeterReturnState cyclicoxiometerStateReturn;
        enumOximeterReturnState manualoxiometerStateReturn;

        //return bpstatus indication state
        BPReturnStates cyclicBPstatusreturn;
        BPReturnStates manualBPstatusreturn;


        //HMI State
        HMISystemState hmiexecstate;

        //Action state for seq innner OXI
        Action actionupdate;

        //Action Update for BP
        Action BPactionupdate;


        //Action for each process
        Action actionoxiometercyclic;
        Action actionoxiometermanual;
        Action actioncyclicdataupdate;
        Action actionBPcyclic;
        Action actionBPmanual;

        //internal flags for requesting
        bool CyclicOxiometerUpdateRequestFlag;
        bool ManualOxiometerUpdateRequestFlag;
        bool CyclicSystemDataUpdateRequestFlag;
        bool CyclicBPUpdateRequestFlag;
        bool ManualBPUpdateRequestFlag;

        //methods
        void CheckButtons();
        void UpdateSequance();
        void UpdateImmediateOxiometerRequest();
        void UpdateCyclicOxiometerData();
        void UpdateCyclicSystemData();
        void ManualupdateOxiometer();
        void UpdateCyclicBPData();
        void ManualupdateBPData();
        void Actions();
        void Transitions();

        
        //class pointer for callback
        MqttControl* mqttcontrol;

        //others
        PushButton pushbutton;
        Buzzer buzzer;
        Led led;
        BatteryHealth batteryhealth;
        ntptime ctime;

        void cyclicupdaterequest();
        void UpdateImmediateBPRequest();
        void updateBP();
        void DisplayUpdate();
        void HardReset();
        void HandleErrors();
        
    protected:
};

#endif