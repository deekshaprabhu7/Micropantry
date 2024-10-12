#include "wifiComm.h"

// Replace with your network credentials
const char* ssid = "xxxxxxx"; //Replace with your ssid
const char* password = "xxxxxxx"; //Replace with your Wifi Password

void wifiComm_init(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println();
  Serial.println(WiFi.localIP());
}

void wifiSetup(void) {

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  wifiComm_init();
}

