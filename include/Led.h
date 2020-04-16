#ifndef _LED_H
#define _LED_H

#include <definition.h>

#define SEQ_BLINK_DELAY 300

class Led{
    public:
        Led();
        Led(int Red, int Green, int Blue);
        ~Led();

        void LedInit(); 
        //clear all
        void clearAllStates();

        //callingled
        void SetCall();
        void ResetCall();

        //data capturing
        void SetDataCapturing();
        void ResetDataCapturing();

        //Errors
        void IndicateERROR(ERRORLevels level);

        //custom led indication
        void CustomBlink(Leds led, int blinkrate, int blinktimes);
        void SetCustomBlink();
        void ResetCustomBlink();

        //update leds
        void update();

    private:
        LedNextState lednextstate;
        ERRORLevels errorlevel;
        Leds customleds;

        int RedLedPin;
        int GreenLedPin;
        int BlueLedPin;

        //custom blink param
        int customblinkrate;
        int customblinktimes;
        int blinktime;
        bool customledstate;
        
        //ledstate
        bool errorledstate;
        bool datacapledstate;

        //timestamps
        long ledtimestamp;
        long cutomtimestamp;
       
        //flags for blinks led
        bool callingflag;
        bool datacapturingflag;
        bool errorindicationflag;
        bool customledindicatorflag;

        /*------------------------Inner Methods-------------------------*/
        //inner methods
        void sequanceblinkupdate();
        void datacapturingupdate();
        void errorsupdate();
        void customblinkupdate();

        //Normal LED ON/OFF
        void SetRedLedON();
        void SetRedLedOFF();
        void SetGreenLedON();
        void SetGreenLedOFF();
        void SetBlueLedON();
        void SetBlueLedOFF();
        //Get LED Status
        bool GetRedLedStatus();
        bool GetGreenLedStatus();
        bool GetBlueLedStatus();
        //Clear All LED @once
        void clearAllLeds();

    protected:

};

#endif