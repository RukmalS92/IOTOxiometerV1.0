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
    BuzzerPin=0;
    buzzerfrquey=0;
    buzzertimestamp=0;

    doccallflag=false;
    assistantcallflag=false;
    outputstatusdoccall=false;
    outputstatusassistcall=false;
    pinMode(Buzzer::BuzzerPin, OUTPUT);
}

//public methods
void Buzzer::SetBuzzerON(){
    digitalWrite(Buzzer::BuzzerPin, _ON_STATUS);
}
void Buzzer::SetBuzzerOFF(){
    digitalWrite(Buzzer::BuzzerPin, _OFF_STATUS);
}
void Buzzer::Update(){
    this->buzzerblinkassitantcall();
    this->buzzerblinkdoccall();
}

//public get methods
bool Buzzer::GetBuzzerStatus(){
    return digitalRead(Buzzer::BuzzerPin);
}


//private
void Buzzer::buzzerblink(int freq, bool* flag, bool* statusflag){
    if((millis() - this->buzzertimestamp) > freq && *flag == true)
    {
         digitalWrite(Buzzer::BuzzerPin, *statusflag);
         this->buzzertimestamp = millis();
         
         #ifdef USE_SERIAL_MONITOR
            Serial.print("Buzzer Status : "); Serial.println(*statusflag);
         #endif
         *statusflag = !*statusflag;
    }
    else if(*flag != true)
    {
        digitalWrite(Buzzer::BuzzerPin, _OFF_STATUS);
        *statusflag = false;
    }
    
}

void Buzzer::buzzerblinkdoccall(){
    this->buzzerblink(_BUZZER_DOC_CALL, &this->doccallflag, &this->outputstatusdoccall);
}

void Buzzer::buzzerblinkassitantcall(){
    this->buzzerblink(_BUZZER_ASSIST_CALL, &this->assistantcallflag, &this->outputstatusassistcall);
}

void Buzzer::SetDocCallBuzzer(){
    this->doccallflag = true;
}
void Buzzer::ClearDocCallBuzzer(){
    this->doccallflag = false;        
}

void Buzzer::SetAssistantCallBuzzer(){
     this->assistantcallflag = true;       
}
void Buzzer::ClearAssistantCallBuzzer(){
     this->assistantcallflag = false;      
}


