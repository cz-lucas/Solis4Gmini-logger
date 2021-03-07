extern "C" {
#include "user_interface.h"
}

#define ledPin 2 // D4

os_timer_t Timer1;
int Counter = 0;

void timerCallback(void *pArg)
{
  *((int *) pArg) += 1;
  digitalWrite(ledPin, !digitalRead(ledPin));
}

void setup() {
  pinMode(ledPin,OUTPUT);
  os_timer_setfn(&Timer1, timerCallback, &Counter);
  os_timer_arm(&Timer1, 1000, true);

}

void loop() {
  yield();

}
