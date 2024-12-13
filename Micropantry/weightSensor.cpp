#include "weightSensor.h"

// HX711 pins
const int HX711_shelf1_dout = D10; // HX711 DOUT pin for shelf1
const int HX711_shelf1_sck = D9; // HX711 SCK pin for shelf1
const int HX711_shelf2_dout = D3; // HX711 DOUT pin for shelf2
const int HX711_shelf2_sck = D2;  // HX711 SCK pin for shelf2
const int HX711_shelf3_dout = D6; // HX711 DOUT pin for shelf3
const int HX711_shelf3_sck = D7;  // HX711 SCK pin for shelf3

unsigned long lastPrintTime = 0; // To track the last print time


// HX711 initialization
HX711_ADC LoadCell_shelf1(HX711_shelf1_dout, HX711_shelf1_sck);
HX711_ADC LoadCell_shelf2(HX711_shelf2_dout, HX711_shelf2_sck);
HX711_ADC LoadCell_shelf3(HX711_shelf3_dout, HX711_shelf3_sck);

volatile bool weightUpdateFlag = false; // Flag for weight update
// const float calibrationFactor_shelf1 = -24.28; // Calibration value for shelf1 lab
// const float calibrationFactor_shelf2 = 22.36; // Calibration value for shelf2 lab
// const float calibrationFactor_shelf3 = 21.12; // Calibration value for shelf3 lab
const float calibrationFactor_shelf1 = -21.98; // Calibration value for shelf1 deploy
const float calibrationFactor_shelf2 = -20.54; // Calibration value for shelf2 deploy
const float calibrationFactor_shelf3 = -21.88; // Calibration value for shelf3 deploy
volatile float currentWeight_shelf1 = 0;      // Current weight on shelf1
volatile float currentWeight_shelf2 = 0;      // Current weight on shelf2
volatile float currentWeight_shelf3 = 0;      // Current weight on shelf3
hw_timer_t *timer = NULL;                     // Timer instance

// Variable to store the cloud data flag or value
volatile bool weightBelowThreshold_shelf1 = false; // Flag for shelf1
volatile bool weightBelowThreshold_shelf2 = false; // Flag for shelf2
volatile bool weightBelowThreshold_shelf3 = false; // Flag for shelf3
volatile float weightToSend_shelf1 = 0;            // Value to send to the cloud for shelf1
volatile float weightToSend_shelf2 = 0;            // Value to send to the cloud for shelf2
volatile float weightToSend_shelf3 = 0;            // Value to send to the cloud for shelf3

volatile float totalWeightToSend = 0;

void IRAM_ATTR onTimer() {
    weightUpdateFlag = true; // Set the flag
}

void weightSensor_init() {
  DEBUG_PRINTLN("Starting weight measurement...");

  // Initialize HX711 for shelf1
  LoadCell_shelf1.begin();
  LoadCell_shelf1.setSamplesInUse(4); // Reduce samples for faster response
  LoadCell_shelf1.start(1000, true); // Initialize with tare for shelf1
  if (LoadCell_shelf1.getTareTimeoutFlag() || LoadCell_shelf1.getSignalTimeoutFlag()) {
    DEBUG_PRINTLN("Timeout for shelf1! Check wiring and connections.");
    while (1);
  } else {
    LoadCell_shelf1.setCalFactor(calibrationFactor_shelf1); // Set the calibration factor for shelf1
    DEBUG_PRINTLN("HX711 for shelf1 is ready.");
  }

  // Initialize HX711 for shelf2
  LoadCell_shelf2.begin();
  LoadCell_shelf2.setSamplesInUse(4); // Reduce samples for faster response
  LoadCell_shelf2.start(1000, true); // Initialize with tare for shelf2
  if (LoadCell_shelf2.getTareTimeoutFlag() || LoadCell_shelf2.getSignalTimeoutFlag()) {
    DEBUG_PRINTLN("Timeout for shelf2! Check wiring and connections.");
    while (1);
  } else {
    LoadCell_shelf2.setCalFactor(calibrationFactor_shelf2); // Set the calibration factor for shelf2
    DEBUG_PRINTLN("HX711 for shelf2 is ready.");
  }

  // Initialize HX711 for shelf3
  LoadCell_shelf3.begin();
  LoadCell_shelf3.setSamplesInUse(4); // Reduce samples for faster response
  LoadCell_shelf3.start(1000, true); // Initialize with tare for shelf3
  if (LoadCell_shelf3.getTareTimeoutFlag() || LoadCell_shelf3.getSignalTimeoutFlag()) {
    DEBUG_PRINTLN("Timeout for shelf3! Check wiring and connections.");
    while (1);
  } else {
    LoadCell_shelf3.setCalFactor(calibrationFactor_shelf2); // Set the calibration factor for shelf2
    DEBUG_PRINTLN("HX711 for shelf3 is ready.");
  }

  // Configure timer interrupt
  // timer = timerBegin(1000000);             // Timer with 1-second resolution
  timer = timerBegin(1000000);             // Timer with 1-second resolution
  timerAttachInterrupt(timer, &onTimer);  // Attach the interrupt handler
  timerAlarm(timer, 10000, true, 0);      // Set interval to 10 ms (100 Hz), enable autoreload, no reload count
  // timerAlarm(timer, 2000000, true, 0);      // Set interval to 10 ms (100 Hz), enable autoreload, no reload count
}

void weightSensor_run() {
  // // Print weights for both shelves
  // DEBUG_PRINT("Shelf1 Weight: ");
  // DEBUG_PRINT(currentWeight_shelf1);
  // DEBUG_PRINTLN(" grams");

  // DEBUG_PRINT("Shelf2 Weight: ");
  // DEBUG_PRINT(currentWeight_shelf2);
  // DEBUG_PRINTLN(" grams");

  if (weightUpdateFlag) {
    weightUpdateFlag = false; // Reset the flag
    if (LoadCell_shelf1.update()) {
      currentWeight_shelf1 = LoadCell_shelf1.getData(); // Update weight for shelf1
    }
    if (LoadCell_shelf2.update()) {
      currentWeight_shelf2 = LoadCell_shelf2.getData(); // Update weight for shelf2
    }
    if (LoadCell_shelf3.update()) {
      currentWeight_shelf3 = LoadCell_shelf3.getData(); // Update weight for shelf3
    }
    
    // Compute the total weight
    totalWeightToSend = currentWeight_shelf1 + currentWeight_shelf2 + currentWeight_shelf3;

    // Print the weights every 2 seconds
    unsigned long currentPrintMillis = millis();
    if (currentPrintMillis - lastPrintTime >= 2000) { // 2 seconds interval
      lastPrintTime = currentPrintMillis; // Update the last print time

      // Print the weights for both shelves
      DEBUG_PRINT("Shelf1 Weight: ");
      DEBUG_PRINT(currentWeight_shelf1);
      DEBUG_PRINT(" grams, ");

      DEBUG_PRINT("Shelf2 Weight: ");
      DEBUG_PRINT(currentWeight_shelf2);
      DEBUG_PRINT(" grams, ");

      DEBUG_PRINT("Shelf3 Weight: ");
      DEBUG_PRINT(currentWeight_shelf3);
      DEBUG_PRINT(" grams");

      DEBUG_PRINT("Total Weight: ");
      DEBUG_PRINT(totalWeightToSend);
      DEBUG_PRINTLN(" grams");
    }
  }

  // Check thresholds for shelf1
  // if (currentWeight_shelf1 < WEIGHTTHRESHOLD) { // Replace 1000 with your threshold for shelf1
  //   weightBelowThreshold_shelf1 = true;
  //   weightToSend_shelf1 = currentWeight_shelf1;
  //   DEBUG_PRINTLN("Shelf1 weight is below threshold. Ready to send to cloud.");
  // } else {
  //   weightBelowThreshold_shelf1 = false;
  // }

  // // Check thresholds for shelf2
  // if (currentWeight_shelf2 < WEIGHTTHRESHOLD) { // Replace 1000 with your threshold for shelf2
  //   weightBelowThreshold_shelf2 = true;
  //   weightToSend_shelf2 = currentWeight_shelf2;
  //   DEBUG_PRINTLN("Shelf2 weight is below threshold. Ready to send to cloud.");
  // } else {
  //   weightBelowThreshold_shelf2 = false;
  // }
}
