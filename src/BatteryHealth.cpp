#include <BatteryHealth.h>
#include <main.h>
#include <Arduino.h>

BatteryHealth::BatteryHealth(){
    BatteryCharge = _BAT_VOLT_SENSE;
    PowerCharging = _CHARGE_VOLT_SENSE;

    
}

BatteryHealth::BatteryHealth(int batterycharge, int powercharging)
{
    BatteryCharge = batterycharge;
    PowerCharging = powercharging;
}

BatteryHealth::~BatteryHealth(){

}

void BatteryHealth::BatteryHealthInit(){
    BatteryVoltageLevel = 0;
    ChargingVoltageLevel = 0;
}

void BatteryHealth::updateBatteryHealth(){
    BatteryVoltageLevel = map(analogRead(BatteryCharge), 0, 4095, 0, OLED_BATTERY_LENGTH);
    #ifdef USE_SERIAL_MONITOR
        //Serial.print("Battery Voltage : ");
        //Serial.println(BatteryVoltageLevel);
    #endif
}

void BatteryHealth::UpdatePowerHealth(){
    ChargingVoltageLevel = map(analogRead(PowerCharging), 0, 4095, 0, OLED_BATTERY_LENGTH);
    #ifdef USE_SERIAL_MONITOR
        //Serial.print("Charge Voltage : ");
        //Serial.println(ChargingVoltageLevel);
    #endif    
}

void BatteryHealth::UpdateBatteryMonitoring(){
    updateBatteryHealth();
    UpdatePowerHealth();
}



/****************************************Getters *****************************************/
int BatteryHealth::GetBatteryHealth(){
    return BatteryVoltageLevel;
}

int BatteryHealth::GetPowerHealth(){
    return ChargingVoltageLevel;
}

