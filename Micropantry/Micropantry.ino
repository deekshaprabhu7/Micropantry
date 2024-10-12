#include "wifiComm.h"
#include "bme688.h"
#include "reedSwitch.h"

void setup()
{
  /* Initialize the communication interfaces */
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Micropantry Sensor Measurement!");

  wifiSetup();

  bme688_init();
  reedSwitch_init();
}

void loop()
{
  bme688_run();
  reedSwitch_run();
}
