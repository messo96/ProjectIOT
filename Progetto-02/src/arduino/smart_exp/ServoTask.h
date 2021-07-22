#ifndef __SERVOTASK__
#define __SERVOTASK__
#include <Arduino.h>
#include "Task.h"
#include "ServoTimer2.h"
#include "MapFloat.h"
#define MAX_VEL 2

class ServoTask : public Task {
    int pin;
    float *vel;
    ServoTimer2 s;
  public:

    ServoTask(int pin, float *vel);
    void on();
    void setPosition(int angle);
    void off();
    void init(int period);
    void tick();

};

#endif
