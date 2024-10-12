#ifndef WIFICOMM_H
#define WIFICOMM_H

#include <WiFi.h>
#include <Arduino.h>

extern const char* ssid;
extern const char* password;

void wifiComm_init(void);
void wifiSetup(void);

#endif