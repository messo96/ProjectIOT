#include "SonarTask.h"

const float vs = 331.45 + 0.62*20;

SonarTask::SonarTask(int pinEcho, int pinTrig) {
  this -> pinTrig = pinTrig;
  this -> pinEcho = pinEcho;

  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

}

float SonarTask:: getDistance()
{
  /* invio impulso */
    digitalWrite(pinTrig,LOW);
    delayMicroseconds(3);
    digitalWrite(pinTrig,HIGH);
    delayMicroseconds(5);
    digitalWrite(pinTrig,LOW);
    
    /* ricevi l’eco */
    float tUS = pulseIn(pinEcho, HIGH);
    float t = tUS / 1000.0 / 1000.0 / 2;
    float d = t*vs * 100; //cm
    return d;
}

void SonarTask :: init(int period) {
  Task::init(period);

}

void SonarTask :: tick() {

}
