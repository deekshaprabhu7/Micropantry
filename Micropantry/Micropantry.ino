#include "bme688.h"

void setup()
{
  /* Initialize the communication interfaces */
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Micropantry Sensor Measurement!");

  bme688_init();
}

void loop()
{
  bme688_run();
}
