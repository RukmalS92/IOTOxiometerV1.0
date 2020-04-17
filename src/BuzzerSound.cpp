#include <BuzzerSound.h>
#include <main.h>
#include <Arduino.h>

Buzzer::Buzzer(){
    Buzzer::BuzzerPin = _BUZZER;
}
Buzzer::Buzzer(int Pin){
    Buzzer::BuzzerPin = Pin;
}
Buzzer::~Buzzer(){

}

//Init
void Buzzer::BuzzerInit(){
    buzzertimestamp=0;
    buzzercustomfreq=0;
    buzzercustombeeptimes=0;
    beeptimes=0;
    custombeep=false;
    doccallflag=false;
    assistantcallflag=false;
    outputstatusdoccall=false;
    outputstatusassistcall=false;
    outputstatusCustomBeep=false;
    pinMode(Buzzer::BuzzerPin, OUTPUT);
}

//public methods
void Buzzer::SetBuzzerON(){
    digitalWrite(Buzzer::BuzzerPin, _ON_STATUS);
}
void Buzzer::SetBuzzerOFF(){
    digitalWrite(Buzzer::BuzzerPin, _OFF_STATUS);
}
//custom beeptimes param set
void Buzzer::SetCustomBeepParameters(int beepfreq, int beeptimes){
    this->buzzercustomfreq = beepfreq;
    this->buzzercustombeeptimes = beeptimes * 2;
}
void Buzzer::Update(){
    this->buzzerblinkassitantcall();
    this->buzzerblinkdoccall();
    this->CustomBuzzerBeep();
}

//public get methods
bool Buzzer::GetBuzzerStatus(){
    return digitalRead(Buzzer::BuzzerPin);
}

//private
void Buzzer::buzzerblink(int freq, bool* flag, bool* statusflag){
    if(*flag == true){
        if((millis() - this->buzzertimestamp) > freq)
        {
            *statusflag = !*statusflag;
            digitalWrite(Buzzer::BuzzerPin, *statusflag);
            this->buzzertimestamp = millis();
            #ifdef USE_SERIAL_MONITOR
                Serial.print("Buzzer Status : "); Serial.println(*statusflag);
            #endif
        }
    }   
    else if(*flag != true)
    {
        if(*statusflag == true){
            digitalWrite(Buzzer::BuzzerPin, _OFF_STATUS);
            *statusflag = false;
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Buzzer Status Clear ");
            #endif
        }
    }
    
}

//custom buzer beep
void Buzzer::CustomBuzzerBeep(){
    if(this->custombeep == true && this->beeptimes < this->buzzercustombeeptimes){
        if(millis() - this->buzzertimestamp > this->buzzercustomfreq){
            this->outputstatusCustomBeep = !this->outputstatusCustomBeep;
            digitalWrite(this->BuzzerPin, this->outputstatusCustomBeep);
            this->buzzertimestamp = millis();
            this->beeptimes++;
            #ifdef USE_SERIAL_MONITOR
                Serial.print("Buzzer Custom Status : "); Serial.println(this->outputstatusCustomBeep);
            #endif
        }
    }
    else{
        if(this->outputstatusCustomBeep == true){
            digitalWrite(this->BuzzerPin, _OFF_STATUS);
            this->outputstatusCustomBeep = false;
        }
        this->beeptimes=0;
        this->custombeep=false;
    }
}

void Buzzer::buzzerblinkdoccall(){
    this->buzzerblink(_BUZZER_DOC_CALL, &this->doccallflag, &this->outputstatusdoccall);
}

void Buzzer::buzzerblinkassitantcall(){
    this->buzzerblink(_BUZZER_ASSIST_CALL, &this->assistantcallflag, &this->outputstatusassistcall);
}

void Buzzer::SetDocCallBuzzer(){
    if(this->doccallflag != true){
        this->doccallflag = true;
    }
}
void Buzzer::ClearDocCallBuzzer(){
    this->doccallflag = false;        
}

void Buzzer::SetAssistantCallBuzzer(){
    if(this->assistantcallflag != true){
        this->assistantcallflag = true;
    }        
}
void Buzzer::ClearAssistantCallBuzzer(){
     this->assistantcallflag = false;      
}

void Buzzer::SetCustomBeepBuzzer(){
    if(this->custombeep != true){
        this->custombeep = true;
    }
}

void Buzzer::ClearCustomBeepBuzzer(){
    this->custombeep = false;
}


