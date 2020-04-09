#ifndef _PUSH_BUTTON_H
#define _PUSH_BUTTON_H

//delay time for debounce
#define _DEBOUNCE_TIME 10
#define _LONGPRESS_TIME 1000

class PushButton{
    public:
        //var
        

       

        //Methods
        PushButton(int mainbutton, int ctrl1button, int ctrl2button);
        PushButton();
        ~PushButton();
        void ButtonInit();
        void UpdateButton(); 
        void ButtonReadMainButton();
        void ButtonReadCTRL1Button();
        void ButtonReadCTRL2Button();

        //Getters
        bool GetMainButtonPressedstate();
        bool GetCtrl1ButtonPressedstate();
        bool GetCtrl2ButtonPressedstate();

        bool GetMainButtonLongPressedstate();
        bool GetCtrl1ButtonLongPressedstate();
        bool GetCtrl2ButtonLongPressedstate();


    private:

        //Flag for Button State Reading
        bool MainButtonPressedFlag;
        bool Ctrl1ButtonPressedFlag;
        bool Ctrl2ButtonPressedFlag;

        bool MainButtonLongPressedFlag;
        bool Ctrl1ButtonLongPressedFlag;
        bool Ctrl2ButtonLongPressedFlag;

        //Pin designators
        int MainButtonPin;
        int CTRL1ButtonPin;
        int CTRL2ButtonPin;

        bool firstcheckflagMainButton;
        bool firstcheckflagCTRL1Button;
        bool firstcheckflagCTRL2Button;
    
        long lastdebouncetimeMainButton;
        long lastdebouncetimeCTRL1Button;
        long lastdebouncetimeCTRL2Button;

        bool laststateMainButton;
        bool laststateCTRL1Button;
        bool laststateCTRL2Button;

        bool mainbuttonvalidpress;
        bool ctrl1buttonvalidpress;
        bool ctrl2buttonvalidpress;

        void ButtonPress(bool* pressedflag, bool* longpressedflag, bool* validpress,
                             bool* firstcheckflag , bool* laststate, int* button, long* lastdebouncetime 
                             );
    

    protected:
        
};

#endif