#ifndef __BLINKLED____
#define __BLINKLED__
#include <Arduino.h>
#include "Task.h"

class BlinkLed{
  int pin, delta = LOW;

public:
  BlinkLed(int pin);
 void blinking(int t);
};

#endif
