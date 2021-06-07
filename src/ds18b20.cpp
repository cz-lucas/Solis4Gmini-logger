#include "ds18b20.h"

#ifdef DS18B20
OneWire  ds(DS18B20_PIN);  // on pin 10 (a 4.7K resistor is necessary)
#endif

myDS18B20::myDS18B20()
{
}

/*
* Source: https://github.com/adlerweb/RM-RF-Module/blob/master/Software/RMRFSensor/RMRFSensor.ino (Modified)
*/

float myDS18B20::getTemperature()
{

#ifdef DS18B20
    byte addr[8];
    byte data[12];
    byte present = 0;
    byte i = 0;
    float celsius;

    ds.reset();
    ds.reset_search();
    ds.search(addr);

    if (addr[0] != 0x28)
        return 85.1;

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);
    delay(775); // conversion takes <=750ms

    present = ds.reset();
    ds.select(addr);
    ds.write(0xBE); // Read Scratchpad

    for (i = 0; i < 9; i++)
    {
        data[i] = ds.read();
    }

    int16_t raw = (data[1] << 8) | data[0];
    celsius = (float)raw / 16.0;

    return (float)(celsius);

#else

    return 127.0;
#endif
}
