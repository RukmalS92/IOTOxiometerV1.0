#include <HMI.h>
#include <Arduino.h>
#include <SevSeg.h>
#include <Push_Button.h>
#include <main.h>
#include <Oled.h>
#include <Mqtt_Control.h>
#include <BatteryHealth.h>
#include <definition.h>
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
    
    mqttcontrol = mqttpointer;
}

HMI::~HMI(){

}

void HMI::AllDevicesInit(){
    Display.DisplayInit();
    spo2status = DEFAULT_STATE;
    spo2currentstate = DONE_DEFAULT_STATE;
    
    CyclicOxiometerUpdateRequestFlag=false;
    CyclicOxiometerUpdateBusyFlag=false;

    CyclicSystemDataUpdatingFlag=false;
        //spo2BPM @ request
    ManualOxiometerUpdateRequestFlag=false;
    ManualOxiometerUpdateBusyFlag=false;

        //BP @ request
    ManaulBPUpdateRequestFlag=false;
    ManaulBPUpdateBusyFlag=false;
        //bool BusBusyFlag;

    I2CBusWriteTimeStamp=0;
    CyclicPatientDataUpdateTimeStamp=0;
    CyclicSystemDataUpdateTimeStamp=0;
        
    batterycharge=0;
    wifisignal=0;


    pushbutton.ButtonInit();
    batteryhealth.BatteryHealthInit();
    buzzer.BuzzerInit();
    led.LedInit();
    delay(2000);
    Display.Display1MonitorSceneSetup();
    Display.Display2MonitorSceneSetup();
    
    /*Here Goes SpO2/BPM Init*/
    /*Here Goes BP Init*/
}

void HMI::HardReset(){
    this->AllDevicesInit();
    mqttcontrol->MqttInit();
}



//update every minute
void HMI::UpdateCyclicSystemData(){
    if(this->CyclicSystemDataUpdatingFlag == true && this->ManaulBPUpdateBusyFlag != true && this->ManualOxiometerUpdateBusyFlag != true){
            if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true){
                #ifdef USE_SERIAL_MONITOR
                    Serial.println("Updating System Data");
                #endif
                Display.SetWifiStreghth(mqttcontrol->GetWifiStrength());
                Display.SetBatteryCharge(batteryhealth.GetBatteryHealth());
                Display.ProcessBatteryHealthDisplay();
                Display.ProcessWifiSignalDisplay();
                this->CyclicSystemDataUpdatingFlag = false;
            }
            else{
                #ifdef USE_SERIAL_MONITOR
                    Serial.println("Updating System Data.....");
                #endif
            }   
    }
    
}

//Update event for every 15min
void HMI::UpdateCyclicOxiometerData(){
    if(this->CyclicOxiometerUpdateRequestFlag == true && this->ManaulBPUpdateBusyFlag != true && this->ManualOxiometerUpdateBusyFlag != true){
        if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true){
            switch (spo2status)
            {
                case TURN_ON:
                    this->CyclicOxiometerUpdateBusyFlag = true;
                    Display.ProcessSetSPO2Display(TURN_ON);
                    break;
                case WAIT_STABLE:
                    Display.ProcessSetSPO2Display(WAIT_STABLE);
                    break;
                case GET_DATA:
                    Display.ProcessSetSPO2Display(GET_DATA);
                    break;
                case TURN_OFF:
                    Display.ProcessSetSPO2Display(TURN_OFF);
                    this->CyclicOxiometerUpdateBusyFlag = false;
                    this->CyclicOxiometerUpdateRequestFlag = false;
                    break;
                
                default:
                    break;
            }
        }
    }        
}

//Manual Update @ Request
void HMI::updateOxiometer(){
    if(this->ManualOxiometerUpdateRequestFlag == true && this->CyclicOxiometerUpdateBusyFlag != true && this->CyclicSystemDataUpdatingFlag != true){
        if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true)
        {
            this->ManualOxiometerUpdateBusyFlag = true;
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Updating Manual SpO2 and BPM.....");
            #endif
            int var = batteryhealth.GetBatteryHealth(); ///TEMP
            
            Display.SetPatientParameters(SPO2, var);
            Display.SetPatientParameters(BPM, var);
            //Display.ProcessSetSPO2Display();
            Display.ProcessSetBPMDisplay();
            this->ManualOxiometerUpdateBusyFlag = false;
            this->ManualOxiometerUpdateRequestFlag = false;
        }
        
    }
}

void HMI::updateBP(){
    if(this->ManaulBPUpdateRequestFlag == true && this->CyclicOxiometerUpdateBusyFlag != true && this->CyclicSystemDataUpdatingFlag != true){
        if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true)
        {
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Updating Manual SpO2 and BPM.....");
            #endif
            int var = batteryhealth.GetBatteryHealth(); ///TEMP
            
            Display.SetPatientParameters(BPsys, var);
            Display.SetPatientParameters(BPdias, var);
            Display.ProcessSetBPressureDisplay();
            this->ManaulBPUpdateBusyFlag = false;
            this->ManaulBPUpdateRequestFlag = false;
        }
        
    }
}
//Update @ request
void HMI::UpdateImmediateOxiometerRequest(){
    this->ManualOxiometerUpdateRequestFlag = true;
}
void HMI::UpdateImmediateBPRequest(){
    this->ManaulBPUpdateRequestFlag = true;
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
    //Show bp
    if(pushbutton.GetCtrl1ButtonPressedstate() == true){
        this->UpdateImmediateBPRequest();
    }
    //Call Assitant Buzzer only
    if(pushbutton.GetCtrl1ButtonLongPressedstate() == true){
        //buzzer.SetAssistantCallBuzzer();
    }
    //show spo2/bpm
    if(pushbutton.GetCtrl2ButtonPressedstate() == true){
        this->UpdateImmediateOxiometerRequest();
    }
    //Reset Assistant call
    if(pushbutton.GetCtrl2ButtonLongPressedstate() == true){
        //buzzer.ClearAssistantCallBuzzer();
    }


}

void HMI::cyclicupdaterequest(){
    if((millis() - HMI::CyclicSystemDataUpdateTimeStamp) > SYSTEM_UPDATE_TIME){
        this->CyclicSystemDataUpdatingFlag = true;
        HMI::CyclicSystemDataUpdateTimeStamp = millis(); 
    }
}

/*--------------------------Main Update---------------------------*/
void HMI::Update(){
    pushbutton.UpdateButton();
    batteryhealth.UpdateBatteryMonitoring();
    buzzer.Update();
    this->cyclicupdaterequest(); //cycle updte request
    this->UpdateCyclicOxiometerData(); //cater according to cyclic update
    this->UpdateCyclicSystemData();
    this->updateOxiometer(); // immediate updating when requested by PUshbutton
    this->updateBP(); //immediate updating when requested by PUshbutton
    this->CheckButtons();
    this->DisplayUpdate();
}

/*------------------------------Setters-----------------------------------*/

void HMI::SetPatientData(enumPatientParam parameter, int value){
    Display.SetPatientParameters(parameter, value);
    mqttcontrol->SetData(parameter, value);
}

void HMI::SetCyclicOxiometerUpdateSeq(enumSpO2Status state){
    this->spo2status = state;
}

void HMI::SetCyclicOxiometerUpdateRequest(){
    this->CyclicOxiometerUpdateRequestFlag = true;
}

/*------------------------------Getters-----------------------------------*/
bool HMI::GetCyclicOxiometerUpdateBusy(){
    return this->CyclicOxiometerUpdateRequestFlag;
}

bool HMI::GetDisplayUpdateBusy(){
    if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true){
        return true;
    }
    else
    {
        return false;
    }
    
}