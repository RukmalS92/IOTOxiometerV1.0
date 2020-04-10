#ifndef _MAIN_H
#define _MAIN_H
/*----ALL Defeined PIN Numbers are GIPOXX------*/

//define all pins
//I2c general

//Max30100
#define _INT_PULSE_METER 34

//blood pressure Monitor
#define _PRESSURE_MOTOR_RUN 21
#define _RELEASE_VALVE 24
#define _PRESSURE_SENSOR 1000 //ANALOG SENSOR

//OLEDS --> uses normal I2C bus


//BUZZER
#define _BUZZER 13

//LED Bulb RGB
#define _RED_LED 27
#define _GREEN_LED 26
#define _BLUE_LED 25

/*Change According to Circuit pullup and pulldown* ----> USED LED and BUZZER*/ 
#define _ON_STATUS HIGH
#define _OFF_STATUS LOW

//PUSH_BUTTON
//pins should be pulledUP @hardware level; 
//when pin pushed down the pin logic leve HIGH --> LOW
#define _MAIN_BUTTON 39
#define _CTRL_1 34 
#define _CTRL_2 35

//Battery Voltage Sense
//when using wifi only ADC1 pins can be used for analog 
//GPIO32-->ADC1_CH4
//GPIO33-->ADC1_CH5
#define _BAT_VOLT_SENSE 32

//charging voltage sense
#define _CHARGE_VOLT_SENSE 33 




/*-----------------------Enable/Disable Serial Print--------------------------------*/
#define USE_SERIAL_MONITOR

/*-----------------------Push Button INT or Normal--------------------------------*/
//#define USE_PB_AS_INTERRUPT

/*----inOLED battery length---*/
#define OLED_BATTERY_LENGTH 25

/*-------------------Update Time Interval-----------------------*/
#define OLED_UPDATE_TIME_INT 10

/*-------------------------------------Update Time Cycle---------------------------------------*/
#define MAIN_CYCLE_UPDATE_TIME 10000

/*-------------------------------------Update System Time Cycle---------------------------------------*/
#define SYSTEM_UPDATE_TIME 5000

/*-------------------------------------I2C Update Delay---------------------------------------*/
#define _I2CBUS_WR_DELAY 0


#endif