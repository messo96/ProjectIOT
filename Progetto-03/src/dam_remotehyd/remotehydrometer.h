#include "SonarTask.h"
#include "HttpConnection.h"
#include "BtConnection.h"
#include "var.h"
#include <SoftwareSerial.h>

#define RX 13
#define TX 15
#define Freq1 100
#define Freq2 200

#define LED_PIN 16 //D0 => 16
#define TRIG_SONAR 5 //D1 => 5
#define ECHO_SONAR 4 //D2 => 4
