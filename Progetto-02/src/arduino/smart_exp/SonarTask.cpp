#include "SonarTask.h"


SonarTask::SonarTask(int pinEcho, int pinTrig, float *pos, float *vel, float *acc, float temp) {
  this -> pinTrig = pinTrig;
  this -> pinEcho = pinEcho;
  this -> pos = pos;
  this -> temp = temp;
  this -> vel = vel;
  this -> acc = acc;
  this -> deltaPos = 0;
  this -> deltaVel = 0;
  this -> posI = 0;
  this -> velI = 0;

  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

}

float SonarTask:: getDistance()
{
  /* invio impulso*/
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(3);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(5);
  digitalWrite(pinTrig, LOW);

  /* ricevi l’eco */
  float tUS = pulseIn(pinEcho, HIGH);
  float t = tUS / 1000.0 / 1000.0 / 2;
  float vs = 331.45 + 0.62 * temp;
  float d = t * vs * 10; //cm to mm
  return d;
}

void SonarTask :: init(int period) {
  Task::init(period);
  this -> freq = period;
  posI = 0;
  velI = 0;
}

void SonarTask :: tick() {
  *pos = getDistance(); 
  deltaPos = *pos - posI;
  *vel = abs(deltaPos) / freq;

  deltaVel = *vel - velI;
  *acc = deltaVel / freq;
  *acc = abs(*acc);

  posI = *pos;
  velI = *vel;

}
