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


void myTicker::FlagChange()
{
    this->flag = true;
}

bool myTicker::getFlag()
{

    return this->flag;
}

void myTicker::setFlagToFalse()
{
    this->flag = false;
}

void callbackFunc(myTicker* instance) {
  instance->FlagChange();
}
/*
Starts ticker
*/

void myTicker::begin()
{
    t->attach(mqttStatusIntveral, callbackFunc, this);
}
