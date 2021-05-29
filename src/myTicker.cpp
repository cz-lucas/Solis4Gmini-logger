#include "myTicker.h"

#include <Ticker.h>

myTicker::myTicker()
{
}

// ####################################### MQTT #######################################################
Ticker mqttStatusTicker;
bool _mqttStatusFlag = false;

void mqttStatusFlagChange()
{
    _mqttStatusFlag = true;
}

bool myTicker::getMqttStatusFlag()
{

    return _mqttStatusFlag;
}

void myTicker::setMqttStatusFlagToFalse()
{
    _mqttStatusFlag = false;
}

// ####################################### READ INVERTER #######################################################
Ticker readTicker;
bool _readFlag = false;

void readFlagChange()
{
    Serial.println("DEBUG 2 ");
    Serial.println(_readFlag);
    _readFlag = true;
}

bool myTicker::getReadFlag()
{

    return _readFlag;
}

void myTicker::setReadFlagToFalse()
{
    _readFlag = false;
}

// ####################################### WRITE INFLUXDB #######################################################
Ticker influxDBTicker;
bool _influxDBFlag = false;

void influxDBFlagChange()
{
    Serial.println("DEBUG 3 ");
    Serial.println(_influxDBFlag);
    _influxDBFlag = true;
}

bool myTicker::getinfluxDBFlag()
{

    return _influxDBFlag;
}

void myTicker::setInfluxDBFlagToFalse()
{
    _influxDBFlag = false;
}

// ####################################### PVOUTPUT #######################################################
Ticker pvoutputTicker;
bool _pvoutputFlag = false;

void pvoutputFlagChange()
{
    Serial.println("DEBUG 4 ");
    Serial.println(_pvoutputFlag);
    _pvoutputFlag = true;
}

bool myTicker::getPVoutputFlag()
{

    return _pvoutputFlag;
}

void myTicker::setPVoutputFlagToFalse()
{
    _pvoutputFlag = false;
}

// ####################################### Other devide #######################################################
#ifdef otherNode
Ticker otherDeviceTicker;
bool _otherDeviceFlag = false;

void otherDeviceFlagChange()
{
    Serial.println("DEBUG 4 ");
    Serial.println(_otherDeviceFlag);
    _otherDeviceFlag = true;
}

bool myTicker::getOtherDeviceFlag()
{

    return _otherDeviceFlag;
}

void myTicker::setOtherDeviceFlagToFalse()
{
    _otherDeviceFlag = false;
}
#endif

/*
Starts ticker
*/
void myTicker::begin()
{
    mqttStatusTicker.attach(mqttStatusIntveral, mqttStatusFlagChange);
    readTicker.attach(readInterval, readFlagChange);
    influxDBTicker.attach(influxdbInterval, influxDBFlagChange);
    pvoutputTicker.attach(PVOUTPUT_sendInterval, pvoutputFlagChange);

#ifdef otherNode
    pvoutputTicker.attach(oInterval, otherDeviceFlagChange);
#endif
}
