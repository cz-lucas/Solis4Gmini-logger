#include "time.h"

WiFiUDP Udp;
unsigned int localPort = 8888;// local port to listen for UDP packets

time_t getNtpTime();
void sendNTPpacket(IPAddress &address);
bool isSummertime(void);

timeControl::timeControl()
{
}

void timeControl::begin()
{
    Udp.begin(localPort);
    Serial.println(Udp.localPort());
    setSyncProvider(getNtpTime);
    setSyncInterval(7200);
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
void timeControl::getTime(uint8_t *_hour, uint8_t *_minute){
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
void timeControl::getDate(uint16_t *_year, uint8_t *_month, uint8_t *_day){
*_year = year();
*_month = month();
*_day = day();
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48;     // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
    IPAddress ntpServerIP; // NTP server's ip address

    while (Udp.parsePacket() > 0)
        ; // discard any previously received packets
    Serial.println("Transmit NTP Request");
    // get a random server from the pool
    WiFi.hostByName(NTP_SERVER_IP, ntpServerIP);
    Serial.print(NTP_SERVER_IP);
    Serial.print(": ");
    Serial.println(ntpServerIP);
    sendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500)
    {
        int size = Udp.parsePacket();
        if (size >= NTP_PACKET_SIZE)
        {
            Serial.println("Receive NTP Response");
            Udp.read(packetBuffer, NTP_PACKET_SIZE); // read packet into the buffer
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900 = (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];
            if (isSummertime() == true)
            {
                return secsSince1900 - 2208988800UL + TIMEZONE * SECS_PER_HOUR + 3600;
            }
            else
            {
                return secsSince1900 - 2208988800UL + TIMEZONE * SECS_PER_HOUR;
            }
        }
    }
    Serial.println("No NTP Response :-(");
    //ESP.reset();
    return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011; // LI, Version, Mode
    packetBuffer[1] = 0;          // Stratum, or type of clock
    packetBuffer[2] = 6;          // Polling Interval
    packetBuffer[3] = 0xEC;       // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123); //NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}