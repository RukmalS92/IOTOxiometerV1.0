#include <Led.h>
#include <main.h>
#include <Arduino.h>
#include <definition.h>

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

    this->lednextstate = redon;
    this->errorlevel = P_NONE;

    customblinkrate=0;
    customblinktimes=0;
    blinktime=0;
    customledstate=false;
        
    errorledstate=false;
    datacapledstate=false;

    ledtimestamp=0;
    cutomtimestamp=0;
       
    callingflag=false;
    datacapturingflag=false;
    errorindicationflag=false;
    customledindicatorflag=false;
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

/*-------------------Special Led Function Sets and Reset--------------------*/
void Led::SetCall(){
    if(this->callingflag != true){
        this->callingflag = true;
    }
}
void Led::ResetCall(){
    this->callingflag = false;
    this->clearAllLeds();
}

void Led::SetDataCapturing(){
    if(this->datacapturingflag != true){
        this->datacapturingflag = true;
    }
    
}
void Led::ResetDataCapturing(){
    this->datacapturingflag = false;
    this->clearAllLeds();
}

void Led::IndicateERROR(ERRORLevels level){
    this->errorlevel = level;
}

void Led::SetCustomBlink(){
    if(this->customledindicatorflag != true){
        this->customledindicatorflag = true;
    }
}
void Led::ResetCustomBlink(){
    this->customledindicatorflag = false;
    this->clearAllLeds();
}

/*--------------------Middle layer finction---------------------*/
void Led::update(){
    this->sequanceblinkupdate();
    this->datacapturingupdate();
    this->errorsupdate();
    this->customblinkupdate();
}

//custom blink
void Led::CustomBlink(Leds led, int blinkrate, int blinktimes){
    this->customleds = led;
    this->customblinkrate = blinkrate;
    this->customblinktimes = blinktimes;
}

/*---------------------Bottom layer Functions--------------------*/
//sequancial blink all leds
void Led::sequanceblinkupdate(){
    if(this->callingflag == true){
        if(millis() - this->ledtimestamp > SEQ_BLINK_DELAY && lednextstate == redon){
            this->SetRedLedON();
            this->SetBlueLedOFF();
            this->SetGreenLedOFF();
            this->ledtimestamp = millis();
            this->lednextstate = blueon;
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Calling Squance Red ON");
            #endif
        }
        else if(millis() - this->ledtimestamp > SEQ_BLINK_DELAY && lednextstate == blueon){
            this->SetRedLedOFF();
            this->SetBlueLedON();
            this->SetGreenLedOFF();
            this->ledtimestamp = millis();
            this->lednextstate = greenon;
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Calling Squance Blue ON");
            #endif
        }
        else if(millis() - this->ledtimestamp > SEQ_BLINK_DELAY && lednextstate == greenon){
            this->SetRedLedOFF();
            this->SetBlueLedOFF();
            this->SetGreenLedON();
            this->ledtimestamp = millis();
            this->lednextstate = redon;
            #ifdef USE_SERIAL_MONITOR
                Serial.println("Calling Squance Green ON");
            #endif
        }
    }
}

//data capturing 
void Led::datacapturingupdate(){
    if(this->datacapturingflag == true){
        if(millis() - this->ledtimestamp > 500){
            digitalWrite(this->GreenLedPin, this->datacapledstate);
            this->datacapledstate = !this->datacapledstate;    
            this->ledtimestamp = millis();
            #ifdef USE_SERIAL_MONITOR
                Serial.print("Dat Capturing Led : "); Serial.println(this->datacapledstate);
            #endif
        }
    } 
}


//erros updating
void Led::errorsupdate(){
    switch (this->errorlevel)
    {
        case P_HIGH:
            /* blink @ 200ms */
            if(millis() - this->ledtimestamp > 200){
                digitalWrite(this->RedLedPin, this->errorledstate);
                this->errorledstate = !this->errorledstate;
                this->ledtimestamp = millis();
                #ifdef USE_SERIAL_MONITOR
                    Serial.print("Error Led HIGH: "); Serial.println(this->errorledstate);
                #endif
            }
            break;
        case P_MID:
            /* blink @ 1000ms */
            if(millis() - this->ledtimestamp > 1000){
                digitalWrite(this->RedLedPin, this->errorledstate);
                this->errorledstate = !this->errorledstate;
                this->ledtimestamp = millis();
                #ifdef USE_SERIAL_MONITOR
                    Serial.print("Error Led MID: "); Serial.println(this->errorledstate);
                #endif
            }
            break;
        case P_LOW:
            /* blink @ 2000ms */
            if(millis() - this->ledtimestamp > 2000){
                digitalWrite(this->RedLedPin, this->errorledstate);
                this->errorledstate = !this->errorledstate;
                this->ledtimestamp = millis();
                #ifdef USE_SERIAL_MONITOR
                    Serial.print("Error Led LOW: "); Serial.println(this->errorledstate);
                #endif
            }
            break;
        case P_NONE:
            /* code */
            if(this->errorledstate == true){
                this->errorledstate = false;
                digitalWrite(this->RedLedPin, this->errorledstate);
            }
            break;
        default:
            break;
    }
}


//custom blinking
void Led::customblinkupdate(){
    if(this->customledindicatorflag == true){
        switch (this->customleds)
            {
            case RED:
                if(millis() - this->cutomtimestamp > this->customblinkrate && this->blinktime < this->customblinktimes){
                    digitalWrite(RedLedPin, this->customledstate);
                    this->customledstate = !this->customledstate;
                    this->blinktime++;
                    this->cutomtimestamp = millis();
                    #ifdef USE_SERIAL_MONITOR
                        Serial.println("Custom Red ON");
                    #endif
                }
                break;
            case BLUE:
                if(millis() - this->cutomtimestamp > this->customblinkrate && this->blinktime < this->customblinktimes){
                    digitalWrite(BlueLedPin, this->customledstate);
                    this->customledstate = !this->customledstate;
                    this->blinktime++;
                    this->cutomtimestamp = millis();
                    #ifdef USE_SERIAL_MONITOR
                        Serial.println("Custom Blue ON");
                    #endif
                }
                break;
            case GREEN:
                if(millis() - this->cutomtimestamp > this->customblinkrate && this->blinktime < this->customblinktimes){
                    digitalWrite(GreenLedPin, this->customledstate);
                    this->customledstate = !this->customledstate;
                    this->blinktime++;
                    this->cutomtimestamp = millis();
                    #ifdef USE_SERIAL_MONITOR
                        Serial.println("Custom Green ON");
                    #endif
                }
                break;
            
            default:
                this->blinktime=0;
                this->customledstate=false;
                break;
            }
    }
}