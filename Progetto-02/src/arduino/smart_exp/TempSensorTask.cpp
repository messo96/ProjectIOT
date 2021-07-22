#include "TempSensorTask.h"

DHT* dht;
TempSensorTask::TempSensorTask(int pin, float *temp){
  dht = new DHT(pin, DHT11);
  dht -> begin();
  pinMode(pin, INPUT);
  value = temp;
}


float TempSensorTask::getValue(){
  return dht -> readTemperature();
}


void TempSensorTask :: init(int period){
  Task::init(period);
}

void TempSensorTask :: tick(){
  *value = getValue();
}
