#include "HttpConnection.h"

WiFiClient wifiClient;

/* wifi network name */
char* ssidName = "Gio";
/* WPA2 PSK password */
char* pwd = "messina96";
/* service IP address */
String address = "http://cedc8ff34ab2.ngrok.io";

HttpConnection::HttpConnection() {
  WiFi.begin(ssidName, pwd);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected: \n local IP: ");
  Serial.println(WiFi.localIP());
}

int HttpConnection :: sendData(float value, String place, String stato) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Error Wifi Connection");
    return -3;
  }

  HTTPClient http;
  http.begin(wifiClient, address + "/api/data");
  http.addHeader("Content-Type", "application/json");
  String msg =
    String("{ \"value\": ") + String(value) +
    ", \"place\": \"" + place +
    "\", \"status\": \"" + stato + "\" }";
  int retCode = http.POST(msg);
  http.end();
  // String payload = http.getString();
  // Serial.println(payload);
  return retCode;
}
