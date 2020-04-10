#include <Controller.h>
#include <Arduino.h>
#include <SevSeg.h>
#include <Push_Button.h>
#include <main.h>
#include <Oled.h>
#include <Mqtt_Control.h>
#include<BatteryHealth.h>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)


//Oled Constructor --INNER--
Adafruit_SSD1306 disp1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 disp2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Oled Display(disp1, disp2);

Controller::Controller(){
    ShowBPFlag = false;
    ShowSpO2BPM = false;
    MainResetFlag = false;
    CyclicPaientDataUpdatingFlag = false;
    CyclicSystemDataUpdatingFlag = false;
    DeviceBusUpdating = false;
    I2CBusWriteTimeStamp = 0;
}

Controller::~Controller(){

}

void Controller::AllDevicesInit(){
    /*Here Goes Display Startup page*/
    Display.DisplayInit(); 
    /*Here Goes SpO2/BPM Init*/
    /*Here Goes BP Init*/
}

void Controller::ReadNonDeviceVars(int batteryHealth, int wifistrength){
    BatteryHealth = batteryHealth;
    wifisignal = wifistrength;
}


//Update event for every 15min
void Controller::UpdateCyclicPatientData(){
    if((millis() - Controller::CyclicPatientDataUpdateTimeStamp) > MAIN_CYCLE_UPDATE_TIME){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating Patient Data");
        #endif
        Display.SetSPO2(&BatteryHealth);
        Display.SetBPM(&BatteryHealth);
        Display.SetBPressure(&BatteryHealth, &BatteryHealth);
        Display.ProcessSetSPO2Display();
        Display.ProcessSetBPMDisplay();
        Display.ProcessSetBPressureDisplay();
        if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true){
            Controller::CyclicPatientDataUpdateTimeStamp = millis();
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Updating Patient Data Comeplete");
            #endif
        }
        else{
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Updating Patient Data.....");
            #endif
        }
        
    }
        
}

//update every minute
void Controller::UpdateCyclicSystemData(){
    if((millis() - Controller::CyclicSystemDataUpdateTimeStamp) > SYSTEM_UPDATE_TIME){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating System Data");
        #endif
        Display.SetBatteryVoltage(&BatteryHealth);
        Display.SetWifiSignal(&BatteryHealth);
        Display.ProcessBatteryHealthDisplay();
        Display.ProcessWifiSignalDisplay();
        if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true){
            Controller::CyclicSystemDataUpdateTimeStamp = millis();
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Updating System Data Comeplete");
            #endif
        }else{
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Updating System Data.....");
            #endif
        }
        
    }
    
}

void Controller::DisplayUpdate(){
    Display.updatedisplay1();
    Display.updatedisplay2();
}

//update @ request
//BP
void Controller::UpdateDisplay1atRequest(){
   
}
//BPM and SpO2
void Controller::UpdateDisplay2atRequest(){
   
}

/*-------------------UPDATE @ ButtonPress----------------------*/
//Should be @ priority Level 1
void Controller::UpdateUserRequestFlags(){
    if(Controller::ShowBPFlag == true){
        CyclicPaientDataUpdatingFlag = false;
        CyclicSystemDataUpdatingFlag = false;
        Controller::UpdateDisplay1atRequest();
        ShowBPFlag = false;
    }
    if(Controller::ShowSpO2BPM == true){
        CyclicPaientDataUpdatingFlag = false;
        CyclicSystemDataUpdatingFlag = false;
        Controller::UpdateDisplay2atRequest();
        ShowSpO2BPM = false;
    }
}

/*--------------------------Main Update---------------------------*/
void Controller::Update(){
    Controller::UpdateCyclicPatientData();
    Controller::UpdateCyclicSystemData();
    Controller::DisplayUpdate();
}