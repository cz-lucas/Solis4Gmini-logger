#pragma once

#include "main.h"

class myTicker
{

private:
    bool flag;
    unsigned int interval;
    Ticker *t;

public:
    myTicker(unsigned int interval);
    ~myTicker();

    void begin();

    bool getFlag();
    void setFlagToFalse();
    void FlagChange();
};
