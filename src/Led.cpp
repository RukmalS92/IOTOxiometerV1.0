#include <Led.h>
#include <main.h>
#include <Arduino.h>

Led::Led(){
    Led::RedLedPin = _RED_LED;
    Led::GreenLedPin = _GREEN_LED;
    Led::BlueLedPin = _BLUE_LED;
}

Led::Led(int Red, int Green, int Blue) {
    Led::RedLedPin = Red;
    Led::GreenLedPin = Green;
    Led::BlueLedPin = Blue;
}

Led::~Led() {
    
}

void Led::LedInit() {
    pinMode(Led::RedLedPin, OUTPUT);
    pinMode(Led::GreenLedPin, OUTPUT);
    pinMode(Led::BlueLedPin, OUTPUT);
}

void Led::SetRedLedON() {
    digitalWrite(Led::RedLedPin, _ON_STATUS);
}

void Led::SetRedLedOFF() {
    digitalWrite(Led::RedLedPin, _OFF_STATUS);
}

void Led::SetGreenLedON() {
    digitalWrite(Led::GreenLedPin, _ON_STATUS);
}

void Led::SetGreenLedOFF() {
    digitalWrite(Led::GreenLedPin, _OFF_STATUS);
}

void Led::SetBlueLedON() {
    digitalWrite(Led::BlueLedPin, _ON_STATUS);
}

void Led::SetBlueLedOFF() {
    digitalWrite(Led::BlueLedPin, _OFF_STATUS);
}

bool Led::GetRedLedStatus() {
    return digitalRead(Led::RedLedPin);
}

bool Led::GetGreenLedStatus() {
    return digitalRead(Led::GreenLedPin);
}

bool Led::GetBlueLedStatus() {
    return digitalRead(Led::BlueLedPin);
}

void Led::clearAllLeds(){
     digitalWrite(Led::RedLedPin, _OFF_STATUS);
     digitalWrite(Led::BlueLedPin, _OFF_STATUS);
     digitalWrite(Led::GreenLedPin, _OFF_STATUS);
}
