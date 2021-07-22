#ifndef __TEMPSENSORTASK__
#define __TEMPSENSORTASK__
#include <Arduino.h>
#include <DHT.h>
#include "Task.h"


class TempSensorTask: public Task {
   int pin; 
   float *value;
 public:
  TempSensorTask(int pin, float *temp);
  float getValue();
  void init(int period);  
  void tick();
};

#endif
