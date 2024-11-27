#include "weightSensor.h"

// HX711 pins
const int HX711_dout = D4; // HX711 DOUT pin
const int HX711_sck = D5;  // HX711 SCK pin

// HX711 initialization
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const float calibrationFactor = -23.7; // Replace with your calibration value
volatile float currentWeight = 0;      // Variable to store weight
hw_timer_t *timer = NULL;              // Timer instance

// Interrupt handler for updating weight
void IRAM_ATTR onTimer() {
  if (LoadCell.update()) {
    currentWeight = LoadCell.getData(); // Update the weight
  }
}

void weightSensor_init() {
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

  // Configure timer interrupt
  timer = timerBegin(1000000);             // Timer with 1-second resolution
  timerAttachInterrupt(timer, &onTimer);  // Attach the interrupt handler
  timerAlarm(timer, 10000, true, 0);      // Set interval to 10 ms (100 Hz), enable autoreload, no reload count
}

void weightSensor_run() {
  // Access the current weight (updated by the timer interrupt)
  DEBUG_PRINT("Weight: ");
  DEBUG_PRINT(currentWeight);
  DEBUG_PRINTLN(" grams");
}
