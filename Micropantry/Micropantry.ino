#include "wifiComm.h"
#include "bme688.h"
#include "reedSwitch.h"
#include "adafruitIO_stream.h"

void setup()
{
  /* Initialize the communication interfaces */
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Micropantry Sensor Measurement!");

  wifiSetup();
  adafruitIO_init();
  bme688_init();
  reedSwitch_init();

    Serial.println("Micropantry Sensor Measurement Setup COMPLETE!");
}

void loop()
{
  bme688_run();
  reedSwitch_run();
  adafruitIO_run();
}
