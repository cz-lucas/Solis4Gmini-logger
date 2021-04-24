#include "pvoutput.h"

#define hostPVOUTPUT "pvoutput.org"

#define path "/service/r2/addstatus.jsp"
#define httpsPort 443

LED led_pvOutput;

bool callhttps();
String reqPath = "";

WiFiClientSecure httpsClient; //Declare object of class WiFiClient

pvoutput::pvoutput()
{
}

/*
Send data to pvoutput
@param _year Year (uint16_t)
@param _month Month (uint8_t)
@param _day day (uint8_t)
@param _hour Hour (uint8_t)
@param _minute Minute (uint8_t)
@param _energyToday (float)
@param _power (float)
@param _dc_u DC Voltage (float)
@param _temperature (float)
*/
bool pvoutput::send(uint16_t _year, uint8_t _month, uint8_t _day, uint8_t _hour, uint8_t _minute, float _energyToday, float _power, float _dc_u, float _temperature)
{
#ifdef PVOUTPUT
    reqPath = path;
    reqPath += "?key="; // Add key
    reqPath += PVOUTPUT_key;
    reqPath += "&sid="; // Add system id
    reqPath += PVOUTPUT_systemID;

    reqPath += "&d="; // Add year
    reqPath += String(_year);

    // Add month
    if (_month < 10)
    {
        reqPath += "0";
        reqPath += String(_month);
    }
    else
    {
        reqPath += String(_month);
    }

    // Add day
    if (_day < 10)
    {
        reqPath += "0";
        reqPath += String(_day);
    }
    else
    {
        reqPath += String(_day);
    }

    reqPath += "&t=";
    // Add Hour
    if (_hour < 10)
    {
        reqPath += "0";
        reqPath += String(_hour);
    }
    else
    {
        reqPath += String(_hour);
    }

    reqPath += ":";

    // Add minute
    if (_minute < 10)
    {
        reqPath += "0";
        reqPath += String(_minute);
    }
    else
    {
        reqPath += String(_minute);
    }

    reqPath += "&v1=";                      // Add energy generation
    reqPath += String(_energyToday * 1000); // kWh to Wh

    reqPath += "&v2="; // Add power
    reqPath += String(_power);

    reqPath += "&v5="; // Add temperature
    reqPath += String(_temperature);

    reqPath += "&v6="; // Add dc voltage
    reqPath += String(_dc_u);

    // Make reqeuest
    return callhttps();
#else
    return true;
#endif
}

bool callhttps()
{

    httpsClient.setInsecure();
    //httpsClient.setTimeout(60);
    delay(10);

    Serial.print("HTTPS Connecting");
    int r = 0; //retry counter
    while (!httpsClient.connect(hostPVOUTPUT, httpsPort) && (r < 30))
    {
        delay(100);
        yield();
        Serial.print(".");
        led_pvOutput.yellowToggle();
        r++;
    }
    Serial.println("Something is connected...");
    if (r == 30)
    {
        Serial.println("Connection failed");
        led_pvOutput.yellowOff();
        return false;
    }
    else
    {
        Serial.println("Connected to web");
    }

    Serial.print("requesting URL: ");
    Serial.println(hostPVOUTPUT + reqPath);

    httpsClient.print(String("GET ") + reqPath + " HTTP/1.1\r\n" +
                      "Host: " + hostPVOUTPUT + "\r\n" +
                      "Connection: close\r\n\r\n");

    Serial.println("request sent");

    while (httpsClient.connected())
    {
        String line = httpsClient.readStringUntil('\n');
        if (line == "\r")
        {
            Serial.println("headers received");
            break;
        }
    }

    httpsClient.stop();
    //httpsClient.stop();
    led_pvOutput.yellowOff();
    return true;
}
