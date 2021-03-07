#ifndef MQTT_H
#define MQTT_H

#include "main.h"


class myMqtt
{
public:
    myMqtt();

    void begin();
    void loop();
    void sendStatus();
    void sendValues(float power, float energyToday, float AC_U, float AC_I, float AC_F, float DC_U, float DC_I, float temperature);

};

#endif