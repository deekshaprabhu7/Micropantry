#ifndef REEDSWITCH_H
#define REEDSWITCH_H

#include <Arduino.h>
#include <queue>
#include "debug.h"

#define REED_SWITCH_PIN D8

extern bool reedSwitchStatus;

// Event structure for the reed switch
struct DoorEvent {
  unsigned long timestamp; // Time of the event (in millis)
  bool state;              // True for CLOSED, False for OPEN
};

// Externally accessible event queue
extern std::queue<DoorEvent> doorEvents;

void reedSwitch_init(void);
void reedSwitch_run(void);

#endif