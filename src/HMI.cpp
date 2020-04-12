#include <HMI.h>
#include <Arduino.h>
#include <SevSeg.h>
#include <Push_Button.h>
#include <main.h>
#include <Oled.h>
#include <Mqtt_Control.h>
#include <BatteryHealth.h>

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

HMI::HMI(MqttControl* mqttpointer){
    ShowBPFlag = false;
    ShowSpO2BPM = false;
    MainResetFlag = false;
    CyclicPaientDataUpdatingFlag = false;
    CyclicSystemDataUpdatingFlag = false;
    DeviceBusUpdating = false;
    I2CBusWriteTimeStamp = 0;

    mqttcontrol = mqttpointer;
}

HMI::~HMI(){

}

void HMI::AllDevicesInit(){
    pushbutton.ButtonInit();
    batteryhealth.BatteryHealthInit();
    buzzer.BuzzerInit();
    led.LedInit();
    Display.DisplayInit(); 
    /*Here Goes SpO2/BPM Init*/
    /*Here Goes BP Init*/
}

void HMI::SetPatientData(int parameter, int value){
    Display.SetPatientParameters(parameter, value);
    mqttcontrol->SetData(parameter, value);
}


//Update event for every 15min
void HMI::UpdateCyclicPatientData(){
    if((millis() - HMI::CyclicPatientDataUpdateTimeStamp) > MAIN_CYCLE_UPDATE_TIME){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating Patient Data");
        #endif
        //int var = batteryhealth.GetBatteryHealth(); ///TEMP
        /*
        Display.SetPatientParameters(SPO2, var);
        Display.SetPatientParameters(BPM, var);
        Display.SetPatientParameters(BPsys, var);
        Display.SetPatientParameters(BPdias, var);*/
        Display.ProcessSetSPO2Display();
        Display.ProcessSetBPMDisplay();
        Display.ProcessSetBPressureDisplay();
        if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true){
            HMI::CyclicPatientDataUpdateTimeStamp = millis();
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
void HMI::UpdateCyclicSystemData(){
    if((millis() - HMI::CyclicSystemDataUpdateTimeStamp) > SYSTEM_UPDATE_TIME){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating System Data");
        #endif
        Display.ProcessBatteryHealthDisplay();
        Display.ProcessWifiSignalDisplay();
        if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true){
            HMI::CyclicSystemDataUpdateTimeStamp = millis();
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

void HMI::DisplayUpdate(){
    Display.updatedisplay1();
    Display.updatedisplay2();
}

void HMI::CheckButtons(){
    //doctor call
    if(pushbutton.GetMainButtonPressedstate() == true){
        mqttcontrol->SetDocCallPublishRequest();
        buzzer.SetDocCallBuzzer();
    }
    //clear doctor call
    if(pushbutton.GetMainButtonLongPressedstate() == true){
        mqttcontrol->ClearDocCallPublishRequest();
        buzzer.ClearDocCallBuzzer();
    }
    //Main Reset
    if(pushbutton.GetCtrl1ButtonLongPressedstate() == true){
        /*Main reset*/
    }
    //Show bp
    if(pushbutton.GetCtrl1ButtonPressedstate() == true){

    }
    //show spo2/bpm
    if(pushbutton.GetCtrl2ButtonPressedstate() == true){

    }
}

/*--------------------------Main Update---------------------------*/
void HMI::Update(){
    pushbutton.UpdateButton();
    batteryhealth.UpdateBatteryMonitoring();
    buzzer.Update();
    this->UpdateCyclicPatientData();
    
    this->CheckButtons();
    //Controller::UpdateCyclicSystemData();
    this->DisplayUpdate();
}