#include <Servo.h>
#include "damcontroller.h"
#include <SoftwareSerial.h>

Servo myServo;
SoftwareSerial mySerial(3, 2);
float value;
bool flagManual;

void setup() {
  Serial.begin(115200);
  while (!Serial) {};
  mySerial.begin(9600);
  Serial.println("DC READY");
  pinMode(LED_PIN, OUTPUT);
  myServo.attach(SERVO_PIN);
  myServo.write(180);
  delay(300);
  myServo.write(0);

}


void loop() {

  while (mySerial.available() > 0) {
    String line = mySerial.readString();

    if (line.indexOf("START") >= 0) {
      flagManual = false;
    }
    else if (line.indexOf("STOP") >= 0) {
      flagManual = true;
    }
    else if (line.indexOf("ALLARME") >= 0 ) {
      value = getValueFromSerial(line);
      blinking();
      int rotation = map(getPercentageP(value), 0, 100, 0, 180);
      if (!flagManual)
        myServo.write(rotation);
      //String data = "ALLARME;Rotation: " + String(rotation) + "\tValue: " + String(value) + "\tPercentage:" + String(getPercentageP(value) ;
      String data = "ALLARME;" + String(value) + ";" + String(getPercentageP(value)) ;

      Serial.println(data);

    }
    else if (line.indexOf("MANUAL") >= 0  && flagManual) {
      digitalWrite(LED_PIN, HIGH);
      value = getValueFromSerial(line);
      int rotation = map(getPercentageP(value), 0, 100, 0, 180);
      myServo.write(rotation);
    }
    else {
      Serial.println(line);
    }

  }


}



float getValueFromSerial(String line) {
  char *str;
  char *s;
  line.toCharArray(str, line.length() + 1); // 0:status ; 1:value
  int index = 0;
  while ((s = strtok_r(str, "; ", &str)) != NULL) { // delimiter is the semicolon
    if (index == 1)
      return atof(s);
    else
      index++;
  }
}



void blinking() {
  digitalWrite(LED_PIN, HIGH);
  delay(250);
  digitalWrite(LED_PIN, LOW);
  delay(250);
}



int getPercentageP(float D) {
  if (D > D2)
    return 0;
  if (D > D2 - DeltaD && D <= D2)
    return 20;
  if (D > D2 - (2 * DeltaD) && D <= D2 - DeltaD)
    return 40;
  if (D > D2 - (3 * DeltaD) && D <= D2 - (2 * DeltaD))
    return 60;
  if (D > D2 - (4 * DeltaD) && D <= D2 - (3 * DeltaD))
    return 80;
  if (D <= D2 * (4 * DeltaD))
    return 100;
}
