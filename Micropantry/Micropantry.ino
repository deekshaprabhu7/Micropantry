#include "wifiComm.h"
#include "bme688.h"
#include "reedSwitch.h"
#include "adafruitIO_stream.h"
#include "weightSensor.h"
#include "mpr121.h"
#include "debug.h"

void setup() {
  /* Initialize the communication interfaces */
  Serial.begin(115200);
  Wire.begin();

  // Wait for Serial only if a monitor is connected
  unsigned long startMillis = millis();
  while (!Serial && millis() - startMillis < 2000) {
    // Wait up to 2 seconds for Serial connection, then continue
  }

  DEBUG_PRINTLN("Micropantry Sensor Measurement!");

  // delay(5000);

  wifiSetup();
  adafruitIO_init();
  bme688_init();
  reedSwitch_init();
  weightSensor_init();
  //mpr121_init();

  // delay(5000);

  DEBUG_PRINTLN("Micropantry Sensor Measurement Setup COMPLETE!");
}

void loop() {
  bme688_run();
  reedSwitch_run();
  adafruitIO_run();
  weightSensor_run();
  //mpr121_run();
}
