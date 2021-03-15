#pragma once
#include "main.h"


class inverter
{
public:
    inverter();

    void begin();
    bool request();
    void getAC(float *__ac_u, float *__ac_i, float *__ac_f);
    void getDC(float *__dc_u, float *__dc_i);
    void getTemperature(float *__temperature);
    void getPower(float *__power);
    void getEnergyToday(float *__energyToday);
    
    bool isInverterReachable();
    bool isSoftRun();
    bool setIsInverterReachableFlagLast(bool _value);
    bool getIsInverterReachableFlagLast();

};
