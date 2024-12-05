#ifndef MPR121_H
#define MPR121_H

#include <Wire.h>
#include "Adafruit_MPR121.h"
#include "debug.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

#define MPR121_INT_PIN D1

void mpr121_init(void);
void mpr121_run(void);

#endif