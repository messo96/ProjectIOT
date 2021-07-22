#ifndef __HTTPCONNECTION__
#define __HTTPCONNECTION__
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

class HttpConnection{


  public:
   HttpConnection();
    int sendData(float value, String place, String stato);
};

#endif
