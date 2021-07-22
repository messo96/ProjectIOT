#include "PrintCinematic.h"


PrintCinematic::PrintCinematic(float *pos, float *vel, float *acc) {
  this -> pos = pos;
  this -> vel = vel;
  this -> acc = acc;

}


void PrintCinematic::init(int period) {
  Task::init(period);
}


void PrintCinematic::tick() {
 
 // Serial.println((String)"DEBUG:"+timeTmp + ":"+ velTmp+ ":"+ posTmp);
  Serial.println((String)"DATA:" + *pos + ":" + *vel + ":" + *acc );
  
}
