#ifndef REEDSWITCH_H
#define REEDSWITCH_H

#include <Arduino.h>
#include <queue>
#include "debug.h"

#define REED_SWITCH1_PIN D8
#define REED_SWITCH2_PIN D5

extern bool reedSwitch1Status;
extern bool reedSwitch2Status;

// Event structure for the reed switch
struct DoorEvent {
  unsigned long timestamp; // Time of the event (in millis)
  bool state;              // True for CLOSED, False for OPEN
};

// Externally accessible event queue
extern std::queue<DoorEvent> door1Events;
extern std::queue<DoorEvent> door2Events;

void reedSwitch_init(void);
void reedSwitch_run(void);

#endif