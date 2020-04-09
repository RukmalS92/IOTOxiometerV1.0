#include <Arduino.h>
#include <Push_Button.h>
#include <main.h>

//default 
PushButton::PushButton(int mainbutton, int ctrl1button, int ctrl2button){
    MainButtonPin = mainbutton;
    CTRL1ButtonPin = ctrl1button;
    CTRL2ButtonPin = ctrl2button;

}

PushButton::PushButton(){
    MainButtonPin = _MAIN_BUTTON;
    CTRL1ButtonPin = _CTRL_1;
    CTRL2ButtonPin = _CTRL_2;

}

PushButton::~PushButton(){
    
}

void PushButton::ButtonInit(){
    #ifdef USE_PB_AS_INTERRUPT
        Serial.println("Using Interrupts @ pushbutton pins")
    #endif

    #ifndef USE_PB_AS_INTERRUPT
        pinMode(MainButtonPin, INPUT); //pulled up
        pinMode(CTRL1ButtonPin, INPUT); //pulled up
        pinMode(CTRL2ButtonPin, INPUT);
    #endif
    MainButtonPressedFlag = false;
    Ctrl1ButtonPressedFlag = false;
    Ctrl2ButtonPressedFlag = false;

    MainButtonLongPressedFlag = false;
    Ctrl1ButtonLongPressedFlag = false;
    Ctrl2ButtonLongPressedFlag = false;

    firstcheckflagMainButton = false;
    firstcheckflagCTRL1Button = false;
    firstcheckflagCTRL2Button = false;
    
    lastdebouncetimeMainButton = 0;
    lastdebouncetimeCTRL1Button = 0;
    lastdebouncetimeCTRL2Button = 0;

    laststateMainButton = false;
    laststateCTRL1Button = false;
    laststateCTRL2Button = false;

    mainbuttonvalidpress = false;
    ctrl1buttonvalidpress = false;
    ctrl2buttonvalidpress = false;
}

//in here the flag us getting set in every 10ms cycle. 
//pin should be pulled up at hardware level
void PushButton::ButtonReadMainButton(){
   PushButton::ButtonPress(&MainButtonPressedFlag, &MainButtonLongPressedFlag, &mainbuttonvalidpress,
                           &firstcheckflagMainButton,&laststateMainButton, &MainButtonPin,&lastdebouncetimeMainButton 
                           );
}


void PushButton::ButtonReadCTRL1Button(){
    PushButton::ButtonPress(&Ctrl1ButtonPressedFlag, &Ctrl1ButtonLongPressedFlag, &ctrl1buttonvalidpress, 
                            &firstcheckflagCTRL1Button, &laststateCTRL1Button, &CTRL1ButtonPin, &lastdebouncetimeCTRL1Button);
                            
}

void PushButton::ButtonReadCTRL2Button(){
    PushButton::ButtonPress(&Ctrl2ButtonPressedFlag, &Ctrl2ButtonLongPressedFlag, &ctrl2buttonvalidpress, 
                            &firstcheckflagCTRL2Button, &laststateCTRL2Button, &CTRL2ButtonPin, &lastdebouncetimeCTRL2Button);
}


void PushButton::ButtonPress(bool* pressedflag, bool* longpressedflag, bool* validpress,
                             bool* firstcheckflag , bool* laststate, int* button, long* lastdebouncetime 
                             ){

    bool reading = digitalRead(*button);
    *pressedflag = false;
    *longpressedflag = false;
    //initial button press
    if ((reading != *laststate) && *firstcheckflag == false){
        *lastdebouncetime = millis();
        *firstcheckflag= true;
    }
    //BUtton valid press
    if(((millis() - *lastdebouncetime) > (_DEBOUNCE_TIME )) && (reading == false) && (*firstcheckflag == true) && (*validpress != true)){
        *validpress = true;
    }
    //not valid press
    else if(((millis() - *lastdebouncetime) > _DEBOUNCE_TIME) && (reading != false) && (*firstcheckflag == true) && (*validpress != true)){
        *firstcheckflag = false;
    }

    //getting lng press or short press
    if(((millis() - *lastdebouncetime) > _LONGPRESS_TIME) && (*validpress == true) && (reading == false)){
        *longpressedflag = true;
        *validpress = false;
        *firstcheckflag = false;
    }
    else if(((millis() - *lastdebouncetime) < _LONGPRESS_TIME) && (*validpress == true) && (reading != false)){
        *pressedflag = true;
        *validpress = false;
        *firstcheckflag = false;
    }
    *laststate = reading;

}

void PushButton::UpdateButton(){
   ButtonReadMainButton();
   ButtonReadCTRL1Button();
   ButtonReadCTRL2Button();
}


/***************************************Getters**************************************/
bool PushButton::GetMainButtonPressedstate(){
    return MainButtonPressedFlag;
}

bool PushButton::GetCtrl1ButtonPressedstate(){
    return Ctrl1ButtonPressedFlag;
}

bool PushButton::GetCtrl2ButtonPressedstate(){
    return Ctrl2ButtonPressedFlag;
}


bool PushButton::GetMainButtonLongPressedstate(){
    return MainButtonLongPressedFlag;
}

bool PushButton::GetCtrl1ButtonLongPressedstate(){
    return Ctrl1ButtonLongPressedFlag;
}

bool PushButton::GetCtrl2ButtonLongPressedstate(){
    return Ctrl2ButtonLongPressedFlag;
}




