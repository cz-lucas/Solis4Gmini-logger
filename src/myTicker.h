#pragma once

#include "main.h"

class myTicker
{

private:
    bool statusFlag;
    unsigned int interval;
    Ticker *t;

public:
    myTicker(unsigned int interval);
    ~myTicker();

    void begin();

    bool getStatusFlag();
    void setStatusFlagToFalse();
    void StatusFlagChange();
};
