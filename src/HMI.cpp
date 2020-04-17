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
#include <ntptime.h>

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
    manualoxiometerstatus = DEFAULT_STATE;
    Lastcyclicoxiometerstatus = DEFAULT_STATE;
    Lastmanualoxiometerstatus = DEFAULT_STATE;

    this->cyclicoxiometerStateReturn = DEFAULT_STATE_RET;
    this->manualoxiometerStateReturn = DEFAULT_STATE_RET;
    
    this->hmiexecstate = enIdle;
    this->actionoxiometercyclic = Idle;
    this->actionoxiometermanual = Idle;

    this->CyclicOxiometerUpdateRequestFlag=false;
    this->ManualOxiometerUpdateRequestFlag=false;
 
    pushbutton.ButtonInit();
    batteryhealth.BatteryHealthInit();
    buzzer.BuzzerInit();
    buzzer.SetCustomBeepParameters(500, 5);
    led.LedInit();
    ctime.Init();
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
    int wifi = mqttcontrol->GetWifiStrength();
    int battery = batteryhealth.GetBatteryHealth();
    if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true){
        this->actioncyclicdataupdate = Busy;
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating System Data");
        #endif
        if(this->actionupdate == Idle){
            Display.SetWifiStreghth(wifi);
            Display.SetBatteryCharge(battery);
            Display.ProcessBatteryHealthDisplay();
            Display.ProcessWifiSignalDisplay();
            this->actionupdate = Busy;
        }
        if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true){
            this->actionupdate = Idle;
            this->actioncyclicdataupdate = Idle;
            this->hmiexecstate = enIdle;
            this->CyclicSystemDataUpdateRequestFlag = false;
        }
    }
}

//Update event for every 15min
void HMI::UpdateCyclicOxiometerData(){
    if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true){
        
        if(this->Lastcyclicoxiometerstatus != this->cyclicoxiometerstatus || this->actionupdate == Busy){
            #ifdef USE_SERIAL_MONITOR
                Serial.println("SYSTEM @ Cyclic Oxiometer Update..");
            #endif
            switch (this->cyclicoxiometerstatus)
            {
                case TURN_ON:
                    this->actionoxiometercyclic = Busy; //settin the action busy state
                    if(this->actionupdate == Idle){
                        Display.ProcessSetSPO2Display(TURN_ON);
                        this->actionupdate = Busy;
                    }
                    if(Display.GetDisplay2UpdateBusy() != true){
                        this->cyclicoxiometerStateReturn = TURN_ON_RET;
                        this->actionupdate = Idle;
                    }
                    break;
                case WAIT_STABLE:
                    if(this->actionupdate == Idle){
                        Display.ProcessSetSPO2Display(WAIT_STABLE);
                        this->actionupdate = Busy;   
                    }
                    if(Display.GetDisplay2UpdateBusy() != true){
                        this->cyclicoxiometerStateReturn = STABLE_RET;
                        this->actionupdate = Idle;
                    }
                    break;
                case GET_DATA:
                    if(this->actionupdate == Idle){
                        Display.SetTimeStamp(ctime.GetTime());
                        Display.ProcessSetSPO2Display(GET_DATA);
                        Display.ProcessTimeStamp();
                        this->actionupdate = Busy; 
                    }
                    if(Display.GetDisplay2UpdateBusy() != true && Display.GetDisplay1UpdateBusy() != true){
                        this->cyclicoxiometerStateReturn = GET_DATA_RET;
                        this->actionupdate = Idle;
                    }
                    break;
                case TURN_OFF:
                    if(this->actionupdate == Idle){
                        Display.ProcessSetSPO2Display(TURN_OFF);
                        this->actionupdate = Busy;
                    }
                    if(Display.GetDisplay2UpdateBusy() != true){
                        this->actionupdate = Idle;
                        this->cyclicoxiometerstatus = DEFAULT_STATE;
                        this->CyclicOxiometerUpdateRequestFlag = false;
                        this->actionoxiometercyclic = Idle; //settin the action Idle state
                        this->hmiexecstate = enIdle;
                        this->cyclicoxiometerStateReturn = DEFAULT_STATE_RET;
                    }
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
        if(this->Lastmanualoxiometerstatus != this->manualoxiometerstatus || this->actionupdate == Busy){
            #ifdef USE_SERIAL_MONITOR
            Serial.println("SYSTEM @ MANUAL Oxiometer Update..");
            #endif
            switch (this->manualoxiometerstatus)
            {
                case TURN_ON:
                    this->actionoxiometermanual = Busy; //Setting Action Busy State
                    if(this->actionupdate == Idle){
                        Display.ProcessSetSPO2Display(TURN_ON);
                        this->actionupdate = Busy;
                    }
                    if(Display.GetDisplay2UpdateBusy() != true){
                        this->manualoxiometerStateReturn = TURN_ON_RET;
                        this->actionupdate = Idle;
                    }
                    break;
                case WAIT_STABLE:
                    if(this->actionupdate == Idle){
                        Display.ProcessSetSPO2Display(WAIT_STABLE);
                        this->actionupdate = Busy;
                    }
                    if(Display.GetDisplay2UpdateBusy() != true){
                        this->manualoxiometerStateReturn = STABLE_RET;
                        this->actionupdate = Idle;
                    }
                    break;
                case GET_DATA:
                    if(this->actionupdate == Idle){
                        Display.SetTimeStamp(ctime.GetTime());
                        Display.ProcessSetSPO2Display(GET_DATA);
                        Display.ProcessTimeStamp();
                        this->actionupdate = Busy;
                    }
                    if(Display.GetDisplay2UpdateBusy() != true && Display.GetDisplay1UpdateBusy() != true){
                        this->manualoxiometerStateReturn = GET_DATA_RET;
                        this->actionupdate = Idle;
                    }
                    break;
                case TURN_OFF:
                    if(this->actionupdate == Idle){
                        Display.ProcessSetSPO2Display(TURN_OFF);
                        this->actionupdate = Busy;
                    }
                    if(Display.GetDisplay2UpdateBusy() != true){
                        this->actionupdate = Idle;
                        this->manualoxiometerstatus = DEFAULT_STATE;
                        this->ManualOxiometerUpdateRequestFlag = false;
                        this->actionoxiometermanual = Idle; //Setting Action Idle State
                        this->hmiexecstate = enIdle;
                        this->manualoxiometerStateReturn = DEFAULT_STATE_RET;
                    }
                    break;
                default:
                    break;
            }
            this->Lastmanualoxiometerstatus = this->manualoxiometerstatus;
        }
    }
}

//update cyclic Bp
void HMI::UpdateCyclicBPData(){
    if(Display.GetDisplay1UpdateBusy() != true && Display.GetDisplay2UpdateBusy() != true){
        if(this->LastCyclicBPStatus != this->CyclicBPStatus || this->BPactionupdate == Busy){
            #ifdef USE_SERIAL_MONITOR
                Serial.println("SYSTEM @ Cyclic Blood Pressure..");
            #endif
            switch (this->CyclicBPStatus)
                {
                case BP_DEVICE_READY:
                    this->actionBPcyclic = Busy;
                    if(this->BPactionupdate == Idle){
                        Display.ProcessSetBPDisplay(BP_DEVICE_READY);
                        this->BPactionupdate = Busy;
                    }
                    if(Display.GetDisplay1UpdateBusy() != true){
                        this->cyclicBPstatusreturn = BP_DEVICE_READY_RET;
                        this->BPactionupdate = Idle;
                    }
                    break;
                case BP_PUMP_STAGE_1:
                    if(this->BPactionupdate == Idle){
                        Display.ProcessSetBPDisplay(BP_PUMP_STAGE_1);
                        this->BPactionupdate = Busy;
                    }
                    if(Display.GetDisplay1UpdateBusy() != true){
                        this->cyclicBPstatusreturn = BP_PUMP_STAGE_1_RET;
                        this->BPactionupdate = Idle;
                    }
                    break;
                case BP_PUMP_STAGE_2:
                    if(this->BPactionupdate == Idle){
                        Display.ProcessSetBPDisplay(BP_PUMP_STAGE_2);
                        this->BPactionupdate = Busy;
                    }
                    if(Display.GetDisplay1UpdateBusy() != true){
                        this->cyclicBPstatusreturn = BP_PUMP_STAGE_2_RET;
                        this->BPactionupdate = Idle;
                    }
                    break;
                case BP_WAIT_STABLE:
                    if(this->BPactionupdate == Idle){
                        Display.ProcessSetBPDisplay(BP_WAIT_STABLE);
                        this->BPactionupdate = Busy;
                    }
                    if(Display.GetDisplay1UpdateBusy() != true){
                        this->cyclicBPstatusreturn = BP_WAIT_STABLE_RET;
                        this->BPactionupdate = Idle;
                    }
                    break;
                case BP_RELEASE_PUMP_1:
                    if(this->BPactionupdate == Idle){
                        Display.ProcessSetBPDisplay(BP_RELEASE_PUMP_1);
                        this->BPactionupdate = Busy;
                    }
                    if(Display.GetDisplay1UpdateBusy() != true){
                        this->cyclicBPstatusreturn = BP_RELEASE_PUMP_1_RET;
                        this->BPactionupdate = Idle;
                    }
                    break;
                case BP_RELEASE_PUMP_2:
                    if(this->BPactionupdate == Idle){
                        Display.ProcessSetBPDisplay(BP_RELEASE_PUMP_2);
                        this->BPactionupdate = Busy;
                    }
                    if(Display.GetDisplay1UpdateBusy() != true){
                        this->cyclicBPstatusreturn = BP_RELEASE_PUMP_2_RET;
                        this->BPactionupdate = Idle;
                    }
                    break;
                case BP_CALCULATING:
                    if(this->BPactionupdate == Idle){
                        /*NEED SET TIME FOR DISPLAY 1*/
                        Display.ProcessSetBPDisplay(BP_CALCULATING);
                        /*PROCESS TIME STAMP*/
                        this->BPactionupdate = Busy;
                    }
                    if(Display.GetDisplay1UpdateBusy() != true){
                        this->cyclicBPstatusreturn = BP_CALCULATING_RET;
                        this->BPactionupdate = Idle;
                    }
                    break;
                case BP_DEVICE_DONE:
                    if(this->BPactionupdate == Idle){
                        Display.ProcessSetBPDisplay(BP_DEVICE_DONE);
                        this->BPactionupdate = Busy;
                    }
                    if(Display.GetDisplay1UpdateBusy() != true){
                        this->actionBPcyclic = Idle;
                        this->hmiexecstate = enIdle;
                        this->CyclicBPStatus = BP_DEFAULT;
                        this->CyclicBPUpdateRequestFlag = false;
                        this->cyclicBPstatusreturn = BP_DEVICE_DONE_RET;
                        this->BPactionupdate = Idle;
                    }
                    break;
                default:
                    break;
                }
                this->LastCyclicBPStatus = this->CyclicBPStatus;
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
        //mqttcontrol->SetDocCallPublishRequest();
        buzzer.SetDocCallBuzzer();
    }
    //clear doctor call
    if(pushbutton.GetMainButtonLongPressedstate() == true){
        //mqttcontrol->ClearDocCallPublishRequest();
        buzzer.ClearDocCallBuzzer();
    }
    //Show bp
    if(pushbutton.GetCtrl1ButtonPressedstate() == true){
        
    }
    //Call Assitant Buzzer only
    if(pushbutton.GetCtrl1ButtonLongPressedstate() == true){
        buzzer.SetAssistantCallBuzzer();
    }
    //show oxiometer
    if(pushbutton.GetCtrl2ButtonPressedstate() == true){
        //this->UpdateImmediateOxiometerRequest();
        buzzer.SetCustomBeepBuzzer();
    }
    //Reset Assistant call
    if(pushbutton.GetCtrl2ButtonLongPressedstate() == true){
        //buzzer.ClearAssistantCallBuzzer();
        buzzer.ClearCustomBeepBuzzer();
    }


}

/*--------------------------Transition Sequance---------------------------*/
void HMI::Transitions(){
    if(this->actionoxiometercyclic == Idle && this->actionoxiometermanual == Idle && this->actionBPcyclic == Idle
                        && this->hmiexecstate == enIdle){
        if(this->CyclicOxiometerUpdateRequestFlag == true){
            this->hmiexecstate = enableOxiometerCyclic;
        }
        else if (this->ManualOxiometerUpdateRequestFlag == true){
            this->hmiexecstate = enableOxiometerManual;
        }
        else if (this->CyclicSystemDataUpdateRequestFlag == true)
        {
            this->hmiexecstate = enableSystemupdate;
        }  
        else if (this->CyclicBPUpdateRequestFlag == true)
        {
            this->hmiexecstate = enableBPCyclic;
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
    case enableSystemupdate:
        this->UpdateCyclicSystemData();
        break;
    case enableBPCyclic:
        this->UpdateCyclicBPData();
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
    ctime.update();
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
    }
    
}

void HMI::SetCyclicSystemDataUpdateRequest(){
    if(this->CyclicSystemDataUpdateRequestFlag != true){
        this->CyclicSystemDataUpdateRequestFlag = true;
    }
}

void HMI::SetCyclicBPUpdateRequest(){
    if(this->CyclicBPUpdateRequestFlag != true){
        this->CyclicBPUpdateRequestFlag = true;
    }
}

/*--------------------------Action Getters------------------------------------------*/
enumOximeterReturnState HMI::GetCyclicOxiometerState(){
    return this->cyclicoxiometerStateReturn;
}
enumOximeterReturnState HMI::GetManualOxiometerState(){
    return this->manualoxiometerStateReturn;
}
BPReturnStates HMI::GetCyclicBPState(){
    return this->cyclicBPstatusreturn;
}

/*------------------------------Getters-----------------------------------*/
bool HMI::GetCyclicOxiometerUpdateBusy(){
    return this->CyclicOxiometerUpdateRequestFlag;
}

//send the immediate oxiometer update request status
bool HMI::GetOxiometerManualUpdateStatus(){
    return this->ManualOxiometerUpdateRequestFlag;
}

bool HMI::GetCyclicBPUpdateBusy(){
    return this->CyclicBPUpdateRequestFlag;
}


bool HMI::GetDisplayUpdateBusy(){
    if(Display.GetDisplay2UpdateBusy() == true || Display.GetDisplay1UpdateBusy() == true){
        return true;
    }
    else 
        return false;
}