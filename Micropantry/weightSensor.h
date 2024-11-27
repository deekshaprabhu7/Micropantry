#ifndef WEIGHTSENSOR_H
#define WEIGHTSENSOR_H

#include <HX711_ADC.h>
#include "Arduino.h"
#include "debug.h"

#define WEIGHTTHRESHOLD 1000

void weightSensor_init(void);
void weightSensor_run(void);

#endif