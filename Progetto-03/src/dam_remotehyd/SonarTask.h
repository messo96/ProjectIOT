#ifndef __SONARTASK__
#define __SONARTASK__
#include <Arduino.h>
#include "Task.h"


class SonarTask: public Task {
int pinEcho, pinTrig;

  public:
    SonarTask(int pinEcho, int pinTrig);
    float getDistance();
    void init(int period);
    void tick();
};

#endif
