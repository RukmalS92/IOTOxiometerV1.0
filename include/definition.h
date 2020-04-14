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
    TURN_ON_RETURN,
    STABLE_RETURN,
    GET_DATA_RETURN,
    TURN_OFF_RETURN,
    DEFAULT_STATE_RETURN
};

/*HMI State Machine States*/
enum HMISystemState{
    enableOxiometerCyclic,
    enableOxiometerManual,
    enIdle
};

/*Action Status*/
enum Action{
    Idle,
    Busy
};


#endif