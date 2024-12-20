#ifndef WEIGHTSENSOR_H
#define WEIGHTSENSOR_H

#include <HX711_ADC.h>
#include "Arduino.h"
#include "debug.h"

#define WEIGHTTHRESHOLD 1000

extern volatile float currentWeight_shelf1;      // Current weight on shelf1
extern volatile float currentWeight_shelf2;      // Current weight on shelf2
extern volatile float currentWeight_shelf3;      // Current weight on shelf3
extern volatile float totalWeightToSend;


void weightSensor_init(void);
void weightSensor_run(void);

#endif