#include "BlinkLed.h"

BlinkLed :: BlinkLed(int pin){
  this -> pin = pin;
}

int tS, tE;
void BlinkLed :: blinking (int t){
  tS = millis();
  do{
  digitalWrite(pin, delta);
  delta = delta == LOW ? HIGH : LOW;
  delay(300);
  tE = millis() - tS;
  }while(tE < t);
    digitalWrite(pin, LOW);

}
