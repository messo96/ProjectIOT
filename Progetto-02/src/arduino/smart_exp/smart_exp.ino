/*       SMART_EXP project #2 - Arduino side
 * author: Giovanni Messina
 * Matr: 0000890082
 * E-mail: giovanni.messina3@studio.unibo.it
 * 
 */


#include "smart_exp.h"

#define SLEEP_TIME 5000
#define MAX_TIME 20000
#define MAX_DISTANCE 400
#define ERROR_TIME 2000
#define MIN_FREQ 1
#define MAX_FREQ 50
#define TIME_CALIBRATION 5


bool isStart, stopBtn, isSleep;

Task* sonar;
Task* temp;
Task* servoM;
Task* printCine;

BlinkLed* blinkLed;
Scheduler sched;
enum State {PRE_EXP, PREPARE_START_EXP, START_EXP, END_EXP} state;

//GLOBAL VAR
float pos, vel, acc, temperature;
int freq = 20;

void setup() {
  Serial.begin(9600);
  //set pins
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(PIN_POT, INPUT);
  pinMode(B_START, INPUT);
  pinMode(B_STOP, INPUT);
  pinMode(PIN_PIR, INPUT);
  //interrupt for PIR detection
  attachInterrupt(digitalPinToInterrupt(PIN_PIR), stopSleep, RISING);
  //interrupt for terminate experiment while is processing
  attachInterrupt(digitalPinToInterrupt(B_STOP), stopEsp, RISING);

  //init Task
  sonar = new SonarTask(PIN_ECHO_SONAR, PIN_TRIG_SONAR, &pos, &vel, &acc, temperature);
  temp = new TempSensorTask(PIN_TEMP, &temperature);
  servoM = new ServoTask(PIN_SERVO, &vel);
  printCine = new PrintCinematic(&pos, &vel, &acc);

  blinkLed = new BlinkLed(L2);

  //Serial.println("CALIBRATE:");

  Serial.print("Calibrating sensor .");
  for (int i = 0; i < TIME_CALIBRATION; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Calibrated!");

  sched.init(freq);
  sched.addTask(sonar);
  sched.addTask(printCine);
  sched.addTask(servoM);
  sched.addTask(temp);

  state = PRE_EXP;

}

void loop() {
  int tStart;
  int tEnd;
  switch (state) {

    case PRE_EXP:

      pirDetection();
      if (!isStart) {
        Serial.println("SLEEP");
        Serial.flush();
        startSleep();
      }
      else {
        state = PREPARE_START_EXP;
      }

      break;

    case PREPARE_START_EXP:

      temp ->tick();
      sonar -> tick();
      if (pos >= MAX_DISTANCE ) {
        Serial.println("WARNING:Don't detect any object");
        blinkLed -> blinking(ERROR_TIME);
        state = END_EXP;
      }
      else {
        pos = 0;
        stopBtn = false;

        freq = map(analogRead(PIN_POT), 0, 1023, MIN_FREQ, MAX_FREQ);
        freq = 1000 / freq; //convert Hz in ms ( 1/freq = sec)
        freq = freq / 10 == 0 ? 1000 : (freq / 10) * 10; //ugly resolve for a mcd scheduler, multiple of 10

        state = START_EXP;
        Serial.println(freq);
      }

      break;

    case START_EXP:

      Serial.println("START:");
      digitalWrite(L1, LOW);
      digitalWrite(L2, HIGH);
      Serial.println((String)"FREQ:" + freq);
      //freq = 100;//DEBUG
      sched.setTime(freq);
      sonar -> init(freq);
      printCine -> init(freq);
      servoM -> init(100);
      temp -> init(freq * 5);
      Serial.flush(); // sometimes serial have random symbol.. with this fix
      tStart = millis();
      
      do {
        sched.schedule();
        tEnd = millis() - tStart;
      }
      while (tEnd < MAX_TIME && !stopBtn);
      state = END_EXP;

      break;

    case END_EXP:

      Serial.println("STOP:");
      while (Serial.readString().indexOf("OK") < 0) {
        digitalWrite(L2, HIGH);
        delay(50);
        digitalWrite(L2, LOW);
        delay(50);
      };
      state = PRE_EXP;
      break;
  }
}

void stopEsp() {
  if (!stopBtn) {
    stopBtn = true;
  }

}


void pirDetection() {
  int tStart;
  int tEnd;
  digitalWrite(L1, HIGH);
  digitalWrite(L2, LOW);
  Serial.println("READY");
  tStart = millis();
  tEnd = 0;
  isStart = false;
  do {
    if (digitalRead(PIN_PIR) == HIGH) {
      tStart = millis();
    }
    else {
      tEnd = millis() - tStart;
    }
    if (digitalRead(B_START) == HIGH) {
      isStart = true;
    }
  } while (tEnd < SLEEP_TIME && !isStart);

}


void startSleep() {
  isSleep = true;
  digitalWrite(L1, LOW);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  /*...*/

}

void stopSleep() {
  if (isSleep) {
    digitalWrite(L1, HIGH);
    sleep_disable();
    isSleep = false;
  }

}
