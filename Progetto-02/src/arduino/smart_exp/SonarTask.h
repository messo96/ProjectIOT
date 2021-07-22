#ifndef __SONARTASK__
#define __SONARTASK__
#include <Arduino.h>
#include "Task.h"


class SonarTask : public Task {

    int pinEcho, pinTrig;
    float *pos, temp, *vel, *acc;
    float deltaPos, deltaVel, posI, velI, freq;

  public:
    SonarTask(int pinEcho, int pinTrig, float *pos, float *vel, float *acc, float temp);
    float getDistance();
    void init(int period);
    void tick();
};

#endif
