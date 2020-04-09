#include <Adafruit_GFX.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include <Oled.h>
#include <main.h>
#include <Fonts/FreeSerif9pt7b.h>

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
    Oled1TimeStamp=0;
    Oled2TimeStamp=0;
    updaterequestdisplay1=false;
    updaterequestdisplay2=false;

    LastSPO2=0;
    LastHeartRate=0;
    LastPressureSystolic=0;
    LastPressureDiastolic=0;
    LastBatteryVoltage=0;
    LastWifisignal=0;

    Display1ConnectionError=false;
    Display2ConnectionError=false;

    #ifdef USE_SERIAL_MONITOR
        Serial.println("System Initializing...");
    #endif
    while((Display2ConnectionError = display2.begin(SSD1306_SWITCHCAPVCC, 0x3C)) != true){
        #ifdef USE_SERIAL_MONITOR
            Serial.print("Connection display-2... "); Serial.println("0x3C"); 
        #endif
    }
    /*
    while((*errorflag = display1.begin(SSD1306_SWITCHCAPVCC, 0x3D)) != true){
        #ifdef USE_SERIAL_MONITOR
            Serial.print("Connection display-1... "); Serial.println(address); 
        #endif
    }*/
    Oled::Display2StartupPage();
    //Oled::Display2StartupPage();
    #ifdef USE_SERIAL_MONITOR
        Serial.println("System Initializing Comeplete!!!");
    #endif
}

//get ERRORS
bool Oled::GetDisplay1ERROR(){
    return Oled::Display1ConnectionError;
}

bool Oled::GetDisplay2ERROR(){
    return Oled::Display2ConnectionError;
}

//Startup Page for Display -2 spo2,BPM
void Oled::Display2StartupPage(){
    display2.display();
    display2.clearDisplay();
    display2.setTextSize(3);
    display2.setCursor(0, 8);
    display2.println("IOT-Oxi");
    display2.setCursor(40, 40);
    display2.println("V1.0");
    display2.display();
}
//Startup Page for Display -2 BP
void Oled::Display1StartupPage(){
    display1.clearDisplay();
    display1.setTextSize(3);
    display1.setCursor(0, 0);
    display1.println("IOT-Oxi");
    display1.setCursor(40, 40);
    display1.println("V1.0");
    display1.display();
}


//Display-2 Scene Setup

void Oled::Display2MonitorSceneSetup(){
    display2.clearDisplay();
    display2.drawRect(90, 4, (OLED_BATTERY_LENGTH + 4), 10, WHITE);
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
    
    Oled::SetWifiSignalDisplay(90);
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
    display1.println("999");
    display1.drawLine(75,32,65,48,WHITE);

    //dias
    display1.setCursor(80, 32);
    display1.println("000");

    
    display1.display();
}

//wifiSignal
void Oled::SetWifiSignalDisplay(int signal){
    if(signal != LastWifisignal){
        if(signal > 0 && signal <= 25){
            display2.fillRect(0,0,4,12, BLACK);
            display2.fillRect(0,12,4,4, WHITE);
            display2.fillRect(6,0,16,16, BLACK);
        }
        else if (signal > 25 && signal <= 50)
        {
            display2.fillRect(0,0,4,12, BLACK);
            display2.fillRect(0,12,4,4, WHITE);
            display2.fillRect(6,0,4,8, BLACK);
            display2.fillRect(6,8,4,8, WHITE);
            display2.fillRect(12,0,10,16, BLACK);
        }
        else if (signal > 50 && signal <= 75)
        {
            display2.fillRect(0,0,4,12, BLACK);
            display2.fillRect(0,12,4,4, WHITE);
            display2.fillRect(6,0,4,8, BLACK);
            display2.fillRect(6,8,4,8, WHITE);
            display2.fillRect(12,0,4,4, BLACK);
            display2.fillRect(12,4,4,12, WHITE);
            display2.fillRect(18,0,4,16, BLACK);
        }
        else if (signal > 75)
        {
            display2.fillRect(0,0,4,12, BLACK);
            display2.fillRect(0,12,4,4, WHITE);
            display2.fillRect(6,0,4,8, BLACK);
            display2.fillRect(6,8,4,8, WHITE);
            display2.fillRect(12,0,4,4, BLACK);
            display2.fillRect(12,4,4,12, WHITE);
            display2.fillRect(18,0,4,16, WHITE);
        }
        Oled::updaterequestdisplay2 = true;
    }
    Oled::LastWifisignal = signal;

}

//Battery Health --> diplay2
void Oled::SetBatteryHealthDisplay(int batteryvoltage){
    if(batteryvoltage < OLED_BATTERY_LENGTH && batteryvoltage != Oled::LastBatteryVoltage){
        display2.fillRect((92 + batteryvoltage), 6, (OLED_BATTERY_LENGTH - batteryvoltage), 6, BLACK);
        display2.fillRect(92, 6, batteryvoltage, 6, WHITE);
        Oled::LastBatteryVoltage = batteryvoltage;
        updaterequestdisplay2 = true;
    }
}

//SPO2 --> display2
void Oled::SetSPO2Display(int SPO2value){
    if(Oled::LastSPO2 != SPO2value ){
        display2.fillRect(6, 32, 56, 32, BLACK);
        display2.setTextSize(3); // Draw 2X-scale text
        display2.setTextColor(SSD1306_WHITE);
        display2.setCursor(6, 32);
        display2.println(SPO2value);
        LastSPO2 = SPO2value;
        updaterequestdisplay2 = true;
    }
}

//BPM --> display2
void Oled::SetBPMDisplay(int HeartRate){
    if(LastHeartRate != HeartRate){
        display2.fillRect(70, 32, 56, 32, BLACK);
        display2.setTextSize(3); // Draw 2X-scale text
        display2.setTextColor(SSD1306_WHITE);
        display2.setCursor(70, 32);
        display2.println(HeartRate);;
        LastHeartRate = HeartRate;
        updaterequestdisplay2 = true;
    }
}

//BP -- >Display1
void Oled::SetBPressureDisplay(int SystolicP, int DiastolicP){
    if(LastPressureSystolic != SystolicP || LastPressureDiastolic != DiastolicP)
    {
        display1.fillRect(30, 32, 32, 16, BLACK);
        display1.fillRect(80, 32, 32, 16, BLACK);
        display1.setTextSize(2); // Draw 2X-scale text
        display1.setTextColor(SSD1306_WHITE);
        display1.setCursor(30, 32);
        display1.println(SystolicP);
        display1.setCursor(80, 32);
        display1.println(DiastolicP);
        LastPressureSystolic = SystolicP;
        LastPressureDiastolic = DiastolicP;
        updaterequestdisplay1 = true;
    }
}

//ShowHeart
void Oled::SetBPMBitMap(){
    display2.drawBitmap(0, 0, heart, 16, 16, 1);
}

//update display1
void Oled::UpdateDisplay1(){
    if(updaterequestdisplay1 == true && (millis()-Oled1TimeStamp) >= OLED_UPDATE_TIME_INT){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating Display...");
        #endif
        display1.display();
        //display2.display();
        updaterequestdisplay1 = false;
        Oled1TimeStamp = millis();
    }
}

void Oled::UpdateDisplay1(int* TimeStamp){
    if(updaterequestdisplay1 == true && (millis() - *TimeStamp) >= OLED_UPDATE_TIME_INT){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating Display...");
        #endif
        display1.display();
        updaterequestdisplay1 = false;
        *TimeStamp = millis();
    }
}

//Update display2
void Oled::UpdateDisplay2(){
    if(updaterequestdisplay2 == true && (millis()-Oled2TimeStamp) >= OLED_UPDATE_TIME_INT){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating Display...");
        #endif
        display2.display();
        updaterequestdisplay2 = false;
        Oled2TimeStamp = millis();
    }
}

void Oled::UpdateDisplay2(int* TimeStamp){
    if(updaterequestdisplay2 == true && (millis() - *TimeStamp) >= OLED_UPDATE_TIME_INT){
        #ifdef USE_SERIAL_MONITOR
            Serial.println("Updating Display...");
        #endif
        display2.display();
        updaterequestdisplay2 = false;
        *TimeStamp = millis();
    }
}


