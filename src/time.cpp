#include "time.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER_IP, TIMEZONE * 3600, 7200);
time_t getNtpTime();

bool isSummertime(void);

timeControl::timeControl()
{
}

void timeControl::begin()
{
    timeClient.begin();
    timeClient.forceUpdate();
    Serial.print("Unix Timestamp: ");
    Serial.println(timeClient.getEpochTime());

    setSyncProvider(getNtpTime);
    setSyncInterval(300);
}

void timeControl::loop()
{
    Serial.println("Starting NTP client");
    timeClient.update();
}

void timeControl::print()
{
    Serial.print(hour());
    Serial.print(":");
    Serial.print(minute());
    Serial.print(":");
    Serial.print(second());
    Serial.print(" ");
    Serial.print(day());
    Serial.print(".");
    Serial.print(month());
    Serial.print(".");
    Serial.print(year());
    Serial.println();
}

bool isSummertime(void)
{
    if (month() < 3 || month() > 10)
        return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
    if (month() > 3 && month() < 10)
        return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
    if ((month() == 3 && (hour() + 24 * day()) >= (1 + TIMEZONE + 24 * (31 - (5 * year() / 4 + 4) % 7))) || (month() == 10 && (hour() + 24 * day()) < (1 + TIMEZONE + 24 * (31 - (5 * year() / 4 + 1) % 7))))
        return true;
    else
        return false;
}

/*
Retrurns time
@param _hour (unit8_t)
@param _minute (unit8_t)
*/
void timeControl::getTime(uint8_t *_hour, uint8_t *_minute)
{
    *_hour = hour();
    *_minute = minute();
    //*_second = second();
}

/*
Retrurns date
@param _year (unit16_t)
@param _month (unit8_t)
@param _day (unit8_t)
*/
void timeControl::getDate(uint16_t *_year, uint8_t *_month, uint8_t *_day)
{
    *_year = year();
    *_month = month();
    *_day = day();
}

/*-------- NTP code ----------*/

time_t getNtpTime()
{
    Serial.print("Unix Timestamp: ");
    Serial.println(timeClient.getEpochTime());
    return timeClient.getEpochTime();
}
