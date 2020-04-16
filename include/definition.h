#ifndef _DEFINITION_H
#define _DEFINITION_H

/*Enums*/
/*parameters defines*/
enum  enumPatientParam{
    SPO2 = 0x01,
    BPM = 0x02,
    BPsys = 0x03,
    BPdias = 0x04
};

/*Mqtt Status*/
enum enumMqttStatus{
    wificonnected = 0x10,
    wificonnectionfail = 0x11,
    serverconnected = 0x12,
    serverconnectionfail = 0x13
};

/*ERROR Codes*/
enum enumERRORcodes{
    
} ;

/*ERROR levels*/
enum ERRORLevels{
    P_HIGH,
    P_MID,
    P_LOW,
    P_NONE
};


/*Oximeter Measurement Stages*/
enum enumOximeterStatus{
    TURN_ON,
    WAIT_STABLE,
    GET_DATA,
    TURN_OFF,
    DEFAULT_STATE
};

/*Oximeter return state*/
enum enumOximeterReturnState{
    TURN_ON_RET,
    STABLE_RET,
    GET_DATA_RET,
    TURN_OFF_RET,
    DEFAULT_STATE_RET
};

/*BP paarameters*/
enum BPStates{
    BP_DEVICE_READY,
    BP_PUMP_STAGE_1,
    BP_PUMP_STAGE_2,
    BP_WAIT_STABLE,
    BP_RELEASE_PUMP_1,
    BP_RELEASE_PUMP_2,
    BP_CALCULATING,
    BP_DEVICE_DONE,
    BP_DEFAULT
};

enum BPReturnStates{
    BP_DEVICE_READY_RET,
    BP_PUMP_STAGE_1_RET,
    BP_PUMP_STAGE_2_RET,
    BP_WAIT_STABLE_RET,
    BP_RELEASE_PUMP_1_RET,
    BP_RELEASE_PUMP_2_RET,
    BP_CALCULATING_RET,
    BP_DEVICE_DONE_RET,
    BP_DEFAULT_RET
};

/*HMI State Machine States*/
enum HMISystemState{
    enableOxiometerCyclic,
    enableOxiometerManual,
    enableSystemupdate,
    enableBPCyclic,
    enableBPManual,
    enIdle
};

/*Action Status*/
enum Action{
    Idle,
    Busy
};

/*--------------LED---------------*/
enum LedNextState{
    redon,
    blueon,
    greenon
};

enum Leds{
    RED,
    BLUE,
    GREEN
};



#endif