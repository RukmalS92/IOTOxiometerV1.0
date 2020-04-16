#ifndef _LED_H
#define _LED_H


class Led{
    public:
        Led();
        Led(int Red, int Green, int Blue);
        ~Led();

        //Normal LED ON/OFF
        void LedInit(); 
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

        //callingled
        void SetCall();
        void ResetCall();

        //normal Blink Leds @ given rate

    private:
        int RedLedPin;
        int GreenLedPin;
        int BlueLedPin;

        //timestamps
        long calltimestamp;
        long redblinktimestamp;
        long greenblinktimestamp;
        long blueblinktimestamp;

        //flags for blinks led
        bool callingflag;


        //inner methods
        void sequanceblink();


    protected:

};

#endif