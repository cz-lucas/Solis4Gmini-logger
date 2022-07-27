#include "myTicker.h"

#include <Ticker.h>

myTicker::myTicker(unsigned int interval)
{
    this->t = new Ticker();
    this->interval = interval;
}

myTicker::~myTicker(){
    t->detach();
    delete t;
}

Ticker mqttStatusTicker;
bool _mqttStatusFlag = false;

void myTicker::StatusFlagChange()
{
    this->statusFlag = true;
}

bool myTicker::getStatusFlag()
{

    return this->statusFlag;
}

void myTicker::setStatusFlagToFalse()
{
    this->statusFlag = false;
}

void callbackFunc(myTicker* instance) {
  instance->StatusFlagChange();
}
/*
Starts ticker
*/

void myTicker::begin()
{
    t->attach(mqttStatusIntveral, callbackFunc, this);
}
