#include "influx.h"

#ifdef influxDB
Influxdb influxdb(INFLUXDB_HOST);
#endif

influx::influx()
{
}

void influx::begin()
{
#ifdef influxDB
    influxdb.setDb(INFLUXDB_DATABASE);
#endif
}

/*
Write values in influxdb
@param power Actual power (float)
@param energyToday Energy today (float)
@param AC_U AC voltage (float)
@param AC_I AC current (float)
@param AC_F AC frequency (float)
@param DC_U DC voltage (float)
@param DC_I DC current (float)
@param temperature temperature (float)
*/
void influx::write(float power, float energyToday, float AC_U, float AC_I, float AC_F, float DC_U, float DC_I, float temperature)
{
    InfluxData row(defaultTableName);

#ifdef INFLUXDB_POWER
    row.addValue("power", power);
#endif

#ifdef INFLUXDB_ENERGYTODAY
    row.addValue("energyToday", energyToday);
#endif

#ifdef INFLUXDB_DC_U
    row.addValue("dc-u", DC_U);
#endif

#ifdef INFLUXDB_DC_I
    row.addValue("dc-i", DC_I);
#endif

#ifdef INFLUXDB_AC_U
    row.addValue("ac-u", AC_U);
#endif

#ifdef INFLUXDB_AC_I
    row.addValue("ac-i", AC_I);
#endif

#ifdef INFLUXDB_AC_F
    row.addValue("ac-f", AC_F);
#endif

#ifdef INFLUXDB_TEMPERATURE
    row.addValue("temperature", temperature);
#endif

#ifdef INFLUXDB_RSSI
    row.addValue("rssi", WiFi.RSSI());
#endif

    influxdb.write(row);
}

void influx::writeEnergy(float energy)
{
#ifdef dailyEnergyInfluxDB
    InfluxData row(deTableName);

    row.addValue("energy", energy);
    influxdb.write(row);
#endif
}