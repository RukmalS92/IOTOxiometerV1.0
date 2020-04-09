#ifndef _LED_H
#define _LED_H


class Led{
    public:
        Led();
        Led(int Red, int Green, int Blue);
        ~Led();

        void LedInit(); 
        void SetRedLedON();
        void SetRedLedOFF();
        void SetGreenLedON();
        void SetGreenLedOFF();
        void SetBlueLedON();
        void SetBlueLedOFF();

        bool GetRedLedStatus();
        bool GetGreenLedStatus();
        bool GetBlueLedStatus();

        void clearAllLeds();

    private:
        int RedLedPin;
        int GreenLedPin;
        int BlueLedPin;

    protected:
};

#endif