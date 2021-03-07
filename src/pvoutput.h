#pragma once
#include "main.h"

class pvoutput
{
public:
    pvoutput();
    bool send(uint16_t _year, uint8_t _month, uint8_t _day, uint8_t _hour, uint8_t _minute, float _energyToday, float _power, float _dc_u, float _temperature);
};