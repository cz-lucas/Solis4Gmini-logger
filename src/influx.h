#pragma once
#include "main.h"

class influx{
    public:
    influx();

    void begin();
    void write(float power, float energyToday, float AC_U, float AC_I, float AC_F, float DC_U, float DC_I, float temperature);
    void writeEnergy(float energy);


};