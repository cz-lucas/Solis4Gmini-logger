#pragma once

#include "main.h"

class myTicker
{
public:
    myTicker();
    void begin();

    bool getMqttStatusFlag();
    void setMqttStatusFlagToFalse();

    bool getReadFlag();
    void setReadFlagToFalse();

    bool getinfluxDBFlag();
    void setInfluxDBFlagToFalse();

    bool getPVoutputFlag();
    void setPVoutputFlagToFalse();

    bool getDS18B20Flag();
    void setDS18B20FlagToFalse();


#ifdef otherNode
    bool getOtherDeviceFlag();
    void setOtherDeviceFlagToFalse();
#endif
};
