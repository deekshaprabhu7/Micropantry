#ifndef REEDSWITCH_H
#define REEDSWITCH_H

#include <Arduino.h>

#define REED_SWITCH_PIN D0

void reedSwitch_init(void);
void reedSwitch_run(void);

#endif