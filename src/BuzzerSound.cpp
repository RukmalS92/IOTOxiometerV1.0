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
    pinMode(Buzzer::BuzzerPin, OUTPUT);
}

//public methods
void Buzzer::SetBuzzerON(){
    digitalWrite(Buzzer::BuzzerPin, _ON_STATUS);
}
void Buzzer::SetBuzzerOFF(){
    digitalWrite(Buzzer::BuzzerPin, _OFF_STATUS);
}

//public get methods
bool Buzzer::GetBuzzerStatus(){
    return digitalRead(Buzzer::BuzzerPin);
}


