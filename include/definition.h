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

/*SpO2 Measurement Stages*/
enum enumSpO2Status{
    TURN_ON,
    WAIT_STABLE,
    GET_DATA,
    TURN_OFF,
    DEFAULT_STATE
};

/*spo2 return state*/
enum enumSpO2ReturnState{
    TURN_ON_DONE,
    STABLE_DONE,
    GET_DATA_DONE,
    TURN_OFF_DONE,
    DONE_DEFAULT_STATE
};

#endif