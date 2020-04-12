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
        void Update();
        bool GetBuzzerStatus();
        
    private:
        int BuzzerPin;
        int buzzerfrquey;
        long buzzertimestamp;

        bool doccallflag;
        bool assistantcallflag;
        bool outputstatusdoccall;
        bool outputstatusassistcall;

        void buzzerblink(int freq, bool* flag,bool* statusflag);
        void buzzerblinkdoccall();
        void buzzerblinkassitantcall();


        

    protected:
};

#endif