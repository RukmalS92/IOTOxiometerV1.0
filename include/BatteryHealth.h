#ifndef _BATTERY_HEALTH_H
#define _BATTERY_HEALTH_H

#include <Arduino.h>
class BatteryHealth{
    public:
        
        BatteryHealth();
        BatteryHealth(int batterycharge, int powercharging);
        ~BatteryHealth();
        void BatteryHealthInit();
        void UpdateBatteryMonitoring();

        int GetBatteryHealth();
        int GetPowerHealth();

    private:
        //Pin designators  
        int BatteryCharge;
        int PowerCharging;

        int BatteryVoltageLevel;
        int ChargingVoltageLevel;

        void updateBatteryHealth();
        void UpdatePowerHealth();

    protected:
        
};

#endif