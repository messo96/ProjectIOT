#ifndef __PRINTCINEMATIC__
#define __PRINTCINEMATIC__
#include <Arduino.h>
#include "Task.h"

class PrintCinematic : public Task {
    float *pos, *vel, *acc;
    
  public:

    PrintCinematic(float *pos, float *vel, float *acc);
    void init(int period);
    void tick();

};

#endif
