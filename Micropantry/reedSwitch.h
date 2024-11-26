#ifndef REEDSWITCH_H
#define REEDSWITCH_H

#include <Arduino.h>
#include "debug.h"

#define REED_SWITCH_PIN D0

extern bool reedSwitchStatus;

void reedSwitch_init(void);
void reedSwitch_run(void);

#endif