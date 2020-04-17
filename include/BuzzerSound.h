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
        //calling dcotor
        void SetDocCallBuzzer();
        void ClearDocCallBuzzer();
        //Assistantcall
        void SetAssistantCallBuzzer();
        void ClearAssistantCallBuzzer();
        //custom buzzer option
        void SetCustomBeepBuzzer();
        void ClearCustomBeepBuzzer();
        void SetCustomBeepParameters(int beepfreq, int beeptimes);

        void Update();
        bool GetBuzzerStatus();
        
    private:
        int BuzzerPin;
        int buzzercustomfreq;
        int buzzercustombeeptimes;
        int beeptimes;
        long buzzertimestamp;

        bool doccallflag;
        bool assistantcallflag;
        bool custombeep;
        bool outputstatusdoccall;
        bool outputstatusassistcall;
        bool outputstatusCustomBeep;

        void buzzerblink(int freq, bool* flag,bool* statusflag);
        void buzzerblinkdoccall();
        void buzzerblinkassitantcall();
        void CustomBuzzerBeep();

        

    protected:
};

#endif