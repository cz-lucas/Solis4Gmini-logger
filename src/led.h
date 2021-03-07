#pragma once
#include "main.h"

class LED
{
public:
    LED();
    void begin();

    void blueToggle();
    void blueOn();
    void blueOff();

    void yellowToggle();
    void yellowOn();
    void yellowOff();

    void enableNightBlink();
    void disableNightblink();
    
};
