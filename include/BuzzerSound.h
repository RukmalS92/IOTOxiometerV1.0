#ifndef _BUZZERSOUND_H
#define _BUZZERSOUND_H



class Buzzer{
    public:
        Buzzer();
        Buzzer(int Pin);
        ~Buzzer();

        void BuzzerInit();
        void SetBuzzerON();
        void SetBuzzerOFF();

        bool GetBuzzerStatus();
        
    private:
        int BuzzerPin;

        

    protected:
};

#endif