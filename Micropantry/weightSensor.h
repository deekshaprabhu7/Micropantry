#ifndef WEIGHTSENSOR_H
#define WEIGHTSENSOR_H

#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

void mpr121_init(void);
void mpr121_run(void);

#endif