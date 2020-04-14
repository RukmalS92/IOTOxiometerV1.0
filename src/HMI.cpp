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
    cyclicoxiometerstatus = DEFAULT_STATE;
    Lastcyclicoxiometerstatus = DEFAULT_STATE;
    manualoxiometerstatus = DEFAULT_STATE;
    Lastmanualoxiometerstatus = DEFAULT_STATE;

    this->cyclicoxiometerStateReturn = DEFAULT_STATE_RETURN;
    this->manualoxiometerStateReturn = DEFAULT_STATE_RETURN;
    
    this->hmiexecstate = enIdle;
    this->actionoxiometercyclic = Idle;
    this->actionoxiometermanual = Idle;

    this->CyclicOxiometerUpdateRequestFlag=false;
    this->ManualOxiometerUpdateRequestFlag=false;
 
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
    
        if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true){
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Updating System Data");
            #endif
            Display.SetWifiStreghth(mqttcontrol->GetWifiStrength());
            Display.SetBatteryCharge(batteryhealth.GetBatteryHealth());
            Display.ProcessBatteryHealthDisplay();
            Display.ProcessWifiSignalDisplay();
        }
        else{
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Updating System Data.....");
            #endif
        }   
    
    
}

//Update event for every 15min
void HMI::UpdateCyclicOxiometerData(){
    if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true){
        if(this->cyclicoxiometerstatus != this->Lastcyclicoxiometerstatus){
            #ifdef USE_SERIAL_MONITOR
                Serial.println("SYSTEM @ Cyclic Update..");
            #endif
            switch (this->cyclicoxiometerstatus)
            {
                case TURN_ON:
                    this->actionoxiometercyclic = Busy; //settin the action busy state
                    Display.ProcessSetSPO2Display(TURN_ON);
                    this->cyclicoxiometerStateReturn = TURN_ON_RETURN;
                    break;
                case WAIT_STABLE:
                    Display.ProcessSetSPO2Display(WAIT_STABLE);
                    this->cyclicoxiometerStateReturn = STABLE_RETURN;
                    break;
                case GET_DATA:
                    Display.ProcessSetSPO2Display(GET_DATA);
                    this->cyclicoxiometerStateReturn = GET_DATA_RETURN;
                    break;
                case TURN_OFF:
                    this->cyclicoxiometerstatus = DEFAULT_STATE;
                    this->actionoxiometercyclic = Idle; //settin the action Idle state
                    this->hmiexecstate = enIdle;
                    this->CyclicOxiometerUpdateRequestFlag = false;
                    Display.ProcessSetSPO2Display(TURN_OFF);
                    this->cyclicoxiometerStateReturn = DEFAULT_STATE_RETURN;
                    break;
                
                default:
                    break;
            }
            this->Lastcyclicoxiometerstatus = this->cyclicoxiometerstatus;
        }
    }       
}

//Manual Update of Oxiometer @ Request
void HMI::ManualupdateOxiometer(){
    if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true)
    {
        if(this->Lastmanualoxiometerstatus != this->manualoxiometerstatus){
                #ifdef USE_SERIAL_MONITOR
                    Serial.println("SYSTEM @ MANUAL Update..");
                #endif
                switch (this->manualoxiometerstatus)
                {
                    case TURN_ON:
                        this->actionoxiometermanual = Busy; //Setting Action Busy State
                        Display.ProcessSetSPO2Display(TURN_ON);
                        this->manualoxiometerStateReturn = TURN_ON_RETURN;
                        break;
                    case WAIT_STABLE:
                        Display.ProcessSetSPO2Display(WAIT_STABLE);
                        this->manualoxiometerStateReturn = STABLE_RETURN;
                        break;
                    case GET_DATA:
                        Display.ProcessSetSPO2Display(GET_DATA);
                        this->manualoxiometerStateReturn = GET_DATA_RETURN;
                        break;
                    case TURN_OFF:
                        this->manualoxiometerstatus = DEFAULT_STATE;
                        this->actionoxiometermanual = Idle; //Setting Action Idle State
                        this->hmiexecstate = enIdle;
                        this->ManualOxiometerUpdateRequestFlag = false;
                        Display.ProcessSetSPO2Display(TURN_OFF);
                        this->manualoxiometerStateReturn =DEFAULT_STATE_RETURN;
                        break;
                    default:
                        break;
                }
                this->Lastmanualoxiometerstatus = this->manualoxiometerstatus;
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
    //Show bp
    if(pushbutton.GetCtrl1ButtonPressedstate() == true){
        
    }
    //Call Assitant Buzzer only
    if(pushbutton.GetCtrl1ButtonLongPressedstate() == true){
        //buzzer.SetAssistantCallBuzzer();
    }
    //show oxiometer
    if(pushbutton.GetCtrl2ButtonPressedstate() == true){
        this->UpdateImmediateOxiometerRequest();
    }
    //Reset Assistant call
    if(pushbutton.GetCtrl2ButtonLongPressedstate() == true){
        //buzzer.ClearAssistantCallBuzzer();
    }


}

/*--------------------------Transition Sequance---------------------------*/
void HMI::Transitions(){
    if(this->actionoxiometercyclic == Idle && this->actionoxiometermanual == Idle && this->hmiexecstate == enIdle){
        if(this->CyclicOxiometerUpdateRequestFlag == true){
            this->hmiexecstate = enableOxiometerCyclic;
        }
        else if (this->ManualOxiometerUpdateRequestFlag == true){
            this->hmiexecstate = enableOxiometerManual;
        }
        
    }
}


/*-------------------------Action Sequance------------------------*/
void HMI::Actions(){
    switch (this->hmiexecstate)
    {
    case enableOxiometerCyclic:
        this->UpdateCyclicOxiometerData();
        break;
    case enableOxiometerManual:
        this->ManualupdateOxiometer();
        break;
    case enIdle:
        #ifdef USE_SERIAL_MONITOR
            Serial.println("SYSTEM IDLE..");
        #endif
    default:
        break;
    }
}

/*------------------------------Comeplete Sequance--------------------------------*/
void HMI::UpdateSequance(){
    this->Transitions();
    this->Actions();
}
/*--------------------------Main Update---------------------------*/
void HMI::Update(){
    pushbutton.UpdateButton();
    batteryhealth.UpdateBatteryMonitoring();
    buzzer.Update();
    this->CheckButtons();
    this->UpdateSequance();
    this->DisplayUpdate();
}

/*------------------------------Data Setters-----------------------------------*/

void HMI::SetPatientData(enumPatientParam parameter, int value){
    Display.SetPatientParameters(parameter, value);
    mqttcontrol->SetData(parameter, value);
}

void HMI::SetCyclicOxiometerUpdateSeq(enumOximeterStatus state){
    this->cyclicoxiometerstatus = state;
}

void HMI::SetManualOxiometerUpdateSeq(enumOximeterStatus state){
    this->manualoxiometerstatus = state;
}

/*------------------------------Requesting Setters-----------------------------------*/

void HMI::SetCyclicOxiometerUpdateRequest(){
    if(this->CyclicOxiometerUpdateRequestFlag != true){
        this->CyclicOxiometerUpdateRequestFlag = true;
    }
}

void HMI::UpdateImmediateOxiometerRequest(){
    if(this->ManualOxiometerUpdateRequestFlag != true){
        this->ManualOxiometerUpdateRequestFlag = true;
        #ifdef USE_SERIAL_MONITOR
            Serial.println("SYSTEM MANUAL REQUEST..");
        #endif
    }
    
}

/*--------------------------Action Getters------------------------------------------*/
enumOximeterReturnState HMI::GetCyclicOxiometerState(){
    return this->cyclicoxiometerStateReturn;
}
enumOximeterReturnState HMI::GetManualOxiometerState(){
    return this->manualoxiometerStateReturn;
}

/*------------------------------Getters-----------------------------------*/
bool HMI::GetCyclicOxiometerUpdateBusy(){
    return this->CyclicOxiometerUpdateRequestFlag;
}

//send the immediate oxiometer update request status
bool HMI::GetOxiometerManualUpdateStatus(){
    return this->ManualOxiometerUpdateRequestFlag;
}


bool HMI::GetDisplayUpdateBusy(){
    if(Display.GetDisplay2UpdateBusy() == true || Display.GetDisplay1UpdateBusy() == true){
        return true;
    }
    else 
        return false;
}