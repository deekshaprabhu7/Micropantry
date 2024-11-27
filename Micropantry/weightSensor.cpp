#include "weightSensor.h"

// Pin configuration
const int HX711_dout = D4; // HX711 DOUT pin
const int HX711_sck = D5;  // HX711 SCK pin

// HX711 initialization
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const float calibrationFactor = -23.7; // Replace with your calibration value

void weightSensor_init()
{
  DEBUG_PRINTLN("Starting weight measurement...");

  // Initialize the HX711
  LoadCell.begin();
  LoadCell.setSamplesInUse(4); // Reduce samples for faster response
  unsigned long stabilizingTime = 1000; // Stabilizing time reduced
  LoadCell.start(stabilizingTime, true); // Initialize with tare

  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    DEBUG_PRINTLN("Timeout! Check wiring and connections.");
    while (1);
  } else {
    LoadCell.setCalFactor(calibrationFactor); // Set the calibration factor
    DEBUG_PRINTLN("HX711 is ready.");
  }
}


void weightSensor_run()
{
  // Update HX711 and get weight
  if (LoadCell.update()) {
    float weight = LoadCell.getData();
    DEBUG_PRINT("Weight: ");
    DEBUG_PRINT(weight);
    DEBUG_PRINTLN(" grams");
  }

  delay(10); // Minimal delay for responsiveness

}

