#include "remotehydrometer.h"



SoftwareSerial mySerial(RX, TX);

SonarTask* sonar;
HttpConnection* conn;
BtConnection* bt;

void setup() {

  pinMode(LED_PIN, OUTPUT);
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  Serial.begin(115200);
  mySerial.begin(9600);
  Serial.println("ESP READY");
  sonar = new SonarTask(ECHO_SONAR, TRIG_SONAR);
  conn = new HttpConnection();
  bt = new BtConnection();
}


void loop() {
  float value = sonar->getDistance();
  Serial.println(value);
  if (value > D2 && value < D1) {   //PRE ALLARME
    while (value > D2 && value < D1) {
      blinking();
      Serial.println("PRE ALLARME");
      mySerial.println("PREALLARME");
      sendBT("PREALLARME", String(value));
      preAllarme(value);
      delay(Freq1);
      value = sonar->getDistance();
    }
  }
  else if (value > D1) {    //NORMALE

    digitalWrite(LED_PIN, LOW);
    Serial.println("NORMALE");
    mySerial.println("NORMALE");
    sendBT("NORMALE", String(value));
  }
  else if (value <= D2) {   //ALLARME
    while (value <= D2) {

      String str = "ALLARME;" + String(value);
      sendBT("ALLARME", String(value));
      char buf[str.length() + 1];
      Serial.println(str);
      str.toCharArray(buf, str.length() + 1);
      mySerial.write(buf);
      allarme(value);
      mySerial.println(bt->receiveStatus()); // send to DC manual open dam
      delay(Freq2);
      value = sonar->getDistance();
    }

  }

  delay(500);
}



void preAllarme(float value) {

  int code = conn->sendData(value, "Sonar", "PRE_ALLARME");
  if (code == 200) {
    Serial.println("ok");
  } else {
    Serial.println("error" + String(code) );
  }

}


void allarme(float value) {
  digitalWrite(LED_PIN, HIGH);
  int code = conn->sendData(value, "Sonar", "ALLARME");
  if (code == 200) {
    Serial.println("ok");
  } else {
    Serial.println("error" + String(code) );
  }

}

void sendBT(String status, String value) {
  bt->sendStatus(status + ";" + value);
}

void blinking() {
  digitalWrite(LED_PIN, HIGH);
  delay(250);
  digitalWrite(LED_PIN, LOW);
  delay(250);
}
