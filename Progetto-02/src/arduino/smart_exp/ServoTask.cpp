#include "ServoTask.h"

ServoTask::ServoTask(int pin, float *vel){
this -> pin = pin;
this -> vel = vel;
this -> on();
//Calibrating (to see if it work correctly)
this -> setPosition(2250);
delay(1000);
this -> setPosition(750);
}

void ServoTask::on(){
  s.attach(pin);
}

//from 0 to 180
void ServoTask::setPosition(int angle){
  if( angle >= 750 && angle <= 2250)
  s.write(angle);
}


void ServoTask::off(){
  s.detach();
}


void ServoTask :: init(int period){
  Task::init(period);
}

void ServoTask :: tick(){
  int value = mapFloat(abs(*vel), 0, MAX_VEL, 750, 2250);
 this -> setPosition(value);

}

  

  
