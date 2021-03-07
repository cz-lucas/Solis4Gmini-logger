#include "led.h"

LED::LED()
{
}

os_timer_t Timer1;
uint8_t Counter = 0;
bool nightBlinkEnabled = false;
volatile uint8_t _cnt = 0;

void blinkTimerCallback(void *pArg)
{
    //volatile uint8_t _cnt = *((int *)pArg);
    if (_cnt < 40)
    {
        _cnt++;
        digitalWrite(LED_BLUE, LOW);
    }
    else
    {
        _cnt = 0;
        digitalWrite(LED_BLUE, HIGH);
    }
    //*((int *)pArg) = _cnt;
}

void LED::begin()
{
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_BLUE, LOW);
}

void LED::blueToggle()
{
    digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
}

void LED::blueOn()
{
    digitalWrite(LED_BLUE, HIGH);
}

void LED::blueOff()
{
    digitalWrite(LED_BLUE, LOW);
}

void LED::yellowToggle()
{
    digitalWrite(LED_YELLOW, !digitalRead(LED_YELLOW));
}

void LED::yellowOn()
{
    digitalWrite(LED_YELLOW, HIGH);
}

void LED::yellowOff()
{
    digitalWrite(LED_YELLOW, LOW);
}

void LED::enableNightBlink()
{
    if (nightBlinkEnabled == false)
    {
        os_timer_setfn(&Timer1, blinkTimerCallback, &Counter);
        Serial.println("Enabled night blink");
        os_timer_arm(&Timer1, 500, true); // Timer1 Interval 0,5s
    }

    nightBlinkEnabled = true;
}

void LED::disableNightblink()
{
    if (nightBlinkEnabled == true)
    {
        os_timer_disarm(&Timer1);
        digitalWrite(LED_BLUE, LOW);
        Serial.println("Disabled night blink");
    }
    nightBlinkEnabled = false;
}