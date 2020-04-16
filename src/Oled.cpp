#include <Adafruit_GFX.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include <Oled.h>
#include <main.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <definition.h>

/*
    display contain 8 pages 0x00 --> 0x07
    page height : 8bits
    text size 1 : 1 page
    text size 2 : 2 apges
*/

static const unsigned char PROGMEM heart[] =
                                   {B00000000, B00000000,
                                    B00000000, B00000000,
                                    B00111100, B00111100,
                                    B01111110, B11111110,
                                    B11111111, B11111111,
                                    B11111111, B11111111,
                                    B01111111, B11111110,
                                    B00111111, B11111100,
                                    B00011111, B11111000,
                                    B00001111, B11110000,
                                    B00000111, B11100000,
                                    B00000011, B11000000,
                                    B00000001, B10000000,
                                    B00000000, B00000000,
                                    B00000000, B00000000,
                                    B00000000, B00000000 };

Oled::Oled(){
    
}

Oled::Oled(Adafruit_SSD1306 d1, Adafruit_SSD1306 d2){
    display1 = d1;
    display2 = d2;
}

Oled::~Oled(){
    
}

//init display
void Oled::DisplayInit(){
    oledtimestamp=0;
    updaterequestdisplay1=false;
    updaterequestdisplay2=false;
    display1updatecomplete=false;
    display2updatecomplete=false;

    LastSPO2=0;
    LastHeartRate=0;
    LastPressureSystolic=0;
    LastPressureDiastolic=0;
    LastBatteryVoltage=0;
    LastWifisignal=0;
    Lasterrorcode=0;

    SpO2=0;
    HeartRate=0;
    PressureSystolic=0;
    PressureDiastolic=0;
    BatteryVoltage=0;
    WifiSignal=0;
    errorcode=0;

    connectiontimeout=0;

    display1connection=false;
    display2connection=false;

    #ifdef USE_SERIAL_MONITOR
        Serial.println("System Initializing...");
    #endif
    Oled::connectiontimeout = millis();
    while(((display2connection = display2.begin(SSD1306_SWITCHCAPVCC, 0x3C)) != true)){
        #ifdef USE_SERIAL_MONITOR
            Serial.print("Connection display-2... "); Serial.println("0x3C"); 
        #endif
        if(((millis() - Oled::connectiontimeout) > _CON_TIMEOUT)) {break;}
    }
    Oled::connectiontimeout = millis();
    while((display1connection = display1.begin(SSD1306_SWITCHCAPVCC, 0x3D)) != true){
        #ifdef USE_SERIAL_MONITOR
            Serial.print("Connection display-1... "); Serial.println("0x3D"); 
        #endif
        if(((millis() - Oled::connectiontimeout) > _CON_TIMEOUT)) {break;}
    }
    if(display2connection == true){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Display 2 Initialized..");
        #endif
        this->Display2StartupPage();    
    }
    else
    {
        Serial.println("Display 2 Damaged");
    }

    if(display1connection == true){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Display 1 Initialized..");
        #endif
        this->Display1StartupPage(); //can refere member function & param like this as well
    }
    else
    {
        Serial.println("Display 1 Damaged");
    }
    #ifdef USE_SERIAL_MONITOR
        Serial.println("System Initializing Comeplete!!!");
    #endif
}


//Startup Page for Display -2 spo2,BPM
void Oled::Display2StartupPage(){
    display2.display();
    display2.clearDisplay();
    display2.setTextColor(SSD1306_WHITE);
    display2.setTextSize(3);
    display2.setCursor(0, 8);
    display2.println("IOT-Oxi");
    display2.setCursor(30, 40);
    display2.println("V1.0");
    display2.display();
}
//Startup Page for Display -2 BP
void Oled::Display1StartupPage(){
    display1.clearDisplay();
    display2.setTextColor(SSD1306_WHITE);
    display1.setTextSize(3);
    display1.setCursor(0, 0);
    display1.println("IOT-Oxi");
    display1.setCursor(30, 40);
    display1.println("V1.0");
    display1.display();
}


//Display-2 Scene Setup

void Oled::Display2MonitorSceneSetup(){
    display2.clearDisplay();
    display2.drawRect(100, 4, (OLED_BATTERY_LENGTH + 4), 10, WHITE);
    display2.setTextColor(SSD1306_WHITE);
    //SpO2
    display2.setTextSize(1); 
    display2.setCursor(2, 16);
    display2.println("SpO2 %");
    display2.setTextSize(3); 
    display2.setCursor(6, 32);
    display2.println("---");
    //BPM
    display2.setTextSize(1); 
    display2.setCursor(60, 16);
    display2.println("BPM");
    display2.setTextSize(3); 
    display2.setCursor(70, 32);
    display2.println("---");


    /********************************/
    /*
        display2.fillRect(0, 0, 90, 15, BLACK);
        display2.setTextSize(1); // Draw 2X-scale text
        display2.setTextColor(SSD1306_WHITE);
        display2.setCursor(0, 0);
        display2.print("EE: ");
        display2.setCursor(20, 0);
        display2.println(255, HEX);*/
        
    /*****************************/
    
    display2.display();
}

//Display-1 Scene Setup
void Oled::Display1MonitorSceneSetup(){
    display1.clearDisplay();
    display1.setTextColor(SSD1306_WHITE);
    //BP
    display1.setTextSize(1); 
    display1.setCursor(2, 16);
    display1.println("BP (Sys/Dias)");
    display1.setTextSize(2); 
    //sys
    display1.setCursor(30, 32);
    display1.println("---");
    display1.drawLine(75,32,65,48,WHITE);

    //dias
    display1.setCursor(80, 32);
    display1.println("---");

    
    display1.display();
}

//wifiSignal --> Display1
void Oled::ProcessWifiSignalDisplay(){
    if(Oled::WifiSignal != LastWifisignal){
        if(Oled::WifiSignal > 0 && Oled::WifiSignal <= 25){
            display1.fillRect(100,0,4,12, BLACK);
            display1.fillRect(100,12,4,4, WHITE);
            display1.fillRect(106,0,16,16, BLACK);
        }
        else if (Oled::WifiSignal > 25 && Oled::WifiSignal <= 50)
        {
            display1.fillRect(100,0,4,12, BLACK);
            display1.fillRect(100,12,4,4, WHITE);
            display1.fillRect(106,0,4,8, BLACK);
            display1.fillRect(106,8,4,8, WHITE);
            display1.fillRect(112,0,10,16, BLACK);
        }
        else if (Oled::WifiSignal > 50 && Oled::WifiSignal <= 75)
        {
            display1.fillRect(100,0,4,12, BLACK);
            display1.fillRect(100,12,4,4, WHITE);
            display1.fillRect(106,0,4,8, BLACK);
            display1.fillRect(106,8,4,8, WHITE);
            display1.fillRect(112,0,4,4, BLACK);
            display1.fillRect(112,4,4,12, WHITE);
            display1.fillRect(118,0,4,16, BLACK);
        }
        else if (Oled::WifiSignal > 75)
        {
            display1.fillRect(100,0,4,12, BLACK);
            display1.fillRect(100,12,4,4, WHITE);
            display1.fillRect(106,0,4,8, BLACK);
            display1.fillRect(106,8,4,8, WHITE);
            display1.fillRect(112,0,4,4, BLACK);
            display1.fillRect(112,4,4,12, WHITE);
            display1.fillRect(118,0,4,16, WHITE);
        }
        Oled::updaterequestdisplay1 = true;
    }
    Oled::LastWifisignal = Oled::WifiSignal;

}

//Battery Health --> diplay2
void Oled::ProcessBatteryHealthDisplay(){
    if(Oled::BatteryVoltage < OLED_BATTERY_LENGTH && Oled::BatteryVoltage != Oled::LastBatteryVoltage){
        display2.fillRect((102 + Oled::BatteryVoltage), 6, (OLED_BATTERY_LENGTH - Oled::BatteryVoltage), 6, BLACK);
        display2.fillRect(102, 6, Oled::BatteryVoltage, 6, WHITE);
        Oled::LastBatteryVoltage = Oled::BatteryVoltage;
        Oled::updaterequestdisplay2 = true;
    }
}

//SPO2 --> display2
void Oled::ProcessSetSPO2Display(enumOximeterStatus state){
    switch (state)
    {
    case TURN_ON:
        display2.fillRect(0, 0, 90, 15, BLACK);
        display2.setTextSize(1); // Draw 2X-scale text
        display2.setTextColor(SSD1306_WHITE);
        display2.setCursor(0, 0);
        display2.print("Sensor ON..");
        this->updaterequestdisplay2 = true;
        break;
    case WAIT_STABLE:
        display2.fillRect(0, 0, 90, 15, BLACK);
        display2.setTextSize(1); // Draw 2X-scale text
        display2.setTextColor(SSD1306_WHITE);
        display2.setCursor(0, 0);
        display2.print("Wait Stab..");
        this->updaterequestdisplay2 = true;
        break;
    case GET_DATA:
        display2.fillRect(0, 0, 90, 15, BLACK);
        display2.setTextSize(1); // Draw 2X-scale text
        display2.setTextColor(SSD1306_WHITE);
        display2.setCursor(0, 0);
        display2.print("Reading..");
        if(Oled::LastSPO2 != Oled::SpO2 || Oled::LastHeartRate != Oled::HeartRate){
            display2.fillRect(6, 32, 56, 32, BLACK);
            display2.fillRect(70, 32, 56, 32, BLACK);
            display2.setTextSize(3); // Draw 2X-scale text
            display2.setTextColor(SSD1306_WHITE);
            display2.setCursor(6, 32);
            display2.println(Oled::SpO2);
            display2.setCursor(70, 32);
            display2.println(this->HeartRate);
            Oled::LastSPO2 = Oled::SpO2;
            Oled::LastHeartRate = Oled::HeartRate;
        }
        this->updaterequestdisplay2 = true;
        break;
    case TURN_OFF:
        display2.fillRect(0, 0, 90, 15, BLACK);
        display2.setTextSize(1); // Draw 2X-scale text
        display2.setTextColor(SSD1306_WHITE);
        display2.setCursor(0, 0);
        display2.print("Sensor OFF...");
        this->updaterequestdisplay2 = true;
        break;    
    default:
        break;
    }  
}


//BP --> display1
void Oled::ProcessSetBPDisplay(BPStates state){
    switch (state)
    {
    case BP_DEVICE_READY:
        display1.fillRect(0, 0, 90, 15, BLACK);
        display1.setTextSize(1); // Draw 2X-scale text
        display1.setTextColor(SSD1306_WHITE);
        display1.setCursor(0, 0);
        display1.print("Device Ready..");
        this->updaterequestdisplay1 = true;
        break;
    case BP_PUMP_STAGE_1:
        display1.fillRect(0, 0, 90, 15, BLACK);
        display1.setTextSize(1); // Draw 2X-scale text
        display1.setTextColor(SSD1306_WHITE);
        display1.setCursor(0, 0);
        display1.print("Pump stg 1..");
        this->updaterequestdisplay1 = true;
        break;
    case BP_PUMP_STAGE_2:
        display1.fillRect(0, 0, 90, 15, BLACK);
        display1.setTextSize(1); // Draw 2X-scale text
        display1.setTextColor(SSD1306_WHITE);
        display1.setCursor(0, 0);
        display1.print("Pump stg 2..");
        this->updaterequestdisplay1 = true;
        break;
    case BP_WAIT_STABLE:
        display1.fillRect(0, 0, 90, 15, BLACK);
        display1.setTextSize(1); // Draw 2X-scale text
        display1.setTextColor(SSD1306_WHITE);
        display1.setCursor(0, 0);
        display1.print("Wait Stable..");
        this->updaterequestdisplay1 = true;
        break;
    case BP_RELEASE_PUMP_1:
        display1.fillRect(0, 0, 90, 15, BLACK);
        display1.setTextSize(1); // Draw 2X-scale text
        display1.setTextColor(SSD1306_WHITE);
        display1.setCursor(0, 0);
        display1.print("Rel Stg 1..");
        this->updaterequestdisplay1 = true;
        break;
    case BP_RELEASE_PUMP_2:
        display1.fillRect(0, 0, 90, 15, BLACK);
        display1.setTextSize(1); // Draw 2X-scale text
        display1.setTextColor(SSD1306_WHITE);
        display1.setCursor(0, 0);
        display1.print("Rel Stg 2..");
        this->updaterequestdisplay1 = true;
        break;
    case BP_CALCULATING:
        display1.fillRect(0, 0, 90, 15, BLACK);
        display1.setTextSize(1); // Draw 2X-scale text
        display1.setTextColor(SSD1306_WHITE);
        display1.setCursor(0, 0);
        display1.print("Calculating..");
        if(Oled::LastPressureSystolic != Oled::PressureSystolic || Oled::LastPressureDiastolic != Oled::PressureDiastolic)
        {
            display1.fillRect(30, 32, 32, 16, BLACK);
            display1.fillRect(80, 32, 32, 16, BLACK);
            display1.setTextSize(2); // Draw 2X-scale text
            display1.setTextColor(SSD1306_WHITE);
            display1.setCursor(30, 32);
            display1.println(Oled::PressureSystolic);
            display1.setCursor(80, 32);
            display1.println(Oled::PressureDiastolic);
            LastPressureSystolic = Oled::PressureSystolic;
            LastPressureDiastolic = Oled::PressureDiastolic;
        }
        this->updaterequestdisplay1 = true;
        break;
    case BP_DEVICE_DONE:
        display1.fillRect(0, 0, 90, 15, BLACK);
        display1.setTextSize(1); // Draw 2X-scale text
        display1.setTextColor(SSD1306_WHITE);
        display1.setCursor(0, 0);
        display1.print("BP Done..");
        this->updaterequestdisplay1 = true;
        break;
    default:
        break;
    }

}

//ShowHeart
void Oled::ProcessSetBPMBitMap(){
    display2.drawBitmap(0, 0, heart, 16, 16, 1);
}

//Show ERROR Code --> Display2
void Oled::ProcessERRORCodeDisplay(){
    if(this->errorcode != this->Lasterrorcode){
        display2.fillRect(0, 0, 90, 15, BLACK);
        display2.setTextSize(1); // Draw 2X-scale text
        display2.setTextColor(SSD1306_WHITE);
        display2.setCursor(0, 0);
        display2.print("EE: ");
        display2.setCursor(50, 0);
        display2.println(this->errorcode, HEX); 
        this->updaterequestdisplay2 = true; 
        this->Lasterrorcode = this->errorcode;  
    }
}

//show Timestamp --> Display1
void Oled::ProcessTimeStamp(){
    display2.fillRect(60, 56, 90, 15, BLACK);
    display2.setTextSize(1); // Draw 2X-scale text
    display2.setTextColor(SSD1306_WHITE);
    display2.setCursor(60, 56);
    display2.print("L.Up: ");
    display2.setCursor(90, 56);
    display2.println(this->timestamp); 
    this->updaterequestdisplay2 = true; 
}

//update display1
void Oled::updatedisplay1(){
    if((updaterequestdisplay1 == true) && ((millis()-oledtimestamp) >= OLED_UPDATE_TIME_INT) && (display1connection == true)){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating Display 1...");
        #endif
        display1.display();
        updaterequestdisplay1 = false;
        oledtimestamp = millis();
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating Display 1 DONE");
        #endif
    }
}

void Oled::updatedisplay1(int* TimeStamp){
    if((updaterequestdisplay1 == true) && ((millis() - *TimeStamp) >= OLED_UPDATE_TIME_INT) && (display1connection == true) ){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating Display 1...");
        #endif
        display1.display();
        updaterequestdisplay1 = false;
        *TimeStamp = millis();
    }
}

//Update display2
void Oled::updatedisplay2(){
    if((updaterequestdisplay2 == true) && ((millis()-oledtimestamp) >= OLED_UPDATE_TIME_INT) && (display2connection == true)){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating Display 2...");
        #endif
        display2.display();
        updaterequestdisplay2 = false;
        oledtimestamp = millis();
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating Display 2 DONE");
        #endif
    }
}

void Oled::updatedisplay2(int* TimeStamp){
    if((updaterequestdisplay2 == true) && ((millis() - *TimeStamp) >= OLED_UPDATE_TIME_INT)  && (display2connection == true)){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating Display 2...");
        #endif
        display2.display();
        updaterequestdisplay2 = false;
        *TimeStamp = millis();
    }
}


/************************************Getters and Setters***********************************************/
//get ERRORS
bool Oled::GetDisplay1ERROR(){
    return Oled::display1connection;
}

bool Oled::GetDisplay2ERROR(){
    return Oled::display2connection;
}

bool Oled::GetDisplay1UpdateBusy(){
    return Oled::updaterequestdisplay1;
}

bool Oled::GetDisplay2UpdateBusy(){
    return Oled::updaterequestdisplay2;
}
//Setters
void Oled::SetPatientParameters(enumPatientParam parameter, int value){
    switch (parameter)
    {
    case SPO2: 
        Oled::SpO2 = value;
        break;
    case BPM : 
        Oled::HeartRate = value;
        break;
    case BPsys : 
        Oled::PressureSystolic = value;
        break;
    case BPdias : 
        Oled::PressureDiastolic = value;
        break;
    default:
        break;
    }
}

void Oled::SetBatteryCharge(int charge){
    this->BatteryVoltage = charge;
}

void Oled::SetWifiStreghth(int strength){
    this->WifiSignal = strength;
}

void Oled::SetERRORCode(int errorcode){
    this->errorcode = errorcode;
}

void Oled::SetTimeStamp(const char* time){
    this->timestamp = time;
}



