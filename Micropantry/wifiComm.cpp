#include "wifiComm.h"

// Replace with your network credentials
const char* ssid = "xxxxxxx"; //Replace with your ssid
const char* password = "xxxxxxx"; //Replace with your Wifi Password

void wifiComm_init(void)
{
  WiFi.mode(WIFI_STA); // Set WiFi to Station mode

  // Get and print the MAC address
  String macAddress = WiFi.macAddress();
  DEBUG_PRINT("MAC Address: ");
  DEBUG_PRINTLN(macAddress);

  WiFi.begin(ssid, password);
  DEBUG_PRINT("Connecting to WiFi ..");
  
  while (WiFi.status() != WL_CONNECTED) {
    DEBUG_PRINT('.');
    delay(1000);
  }
  
  DEBUG_PRINTLN();
  DEBUG_PRINTLN("Connected to WiFi!");
  DEBUG_PRINT("IP Address: ");
  DEBUG_PRINTLN(WiFi.localIP());
}

void wifiSetup(void) {

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  wifiComm_init();
}

