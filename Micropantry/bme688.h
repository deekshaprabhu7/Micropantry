#ifndef BME688_H
#define BME688_H
 
#include <Arduino.h>
#include <Wire.h>
#include <bsec2.h>

extern float temperature;

/* Macros used */
#define PANIC_LED   LED_BUILTIN
#define ERROR_DUR   1000

#define SAMPLE_RATE		BSEC_SAMPLE_RATE_LP    //ToDo: Change this to BSEC_SAMPLE_RATE_ULP later to reduce frequency once in 300s

void bme688_init(void);
void bme688_run(void);


/* Helper functions declarations */
/**
 * @brief : This function toggles the led when a fault was detected
 */
void errLeds(void);

/**
 * @brief : This function checks the BSEC status, prints the respective error code. Halts in case of error
 * @param[in] bsec  : Bsec2 class object
 */
void checkBsecStatus(Bsec2 bsec);

/**
 * @brief : This function is called by the BSEC library when a new output is available
 * @param[in] input     : BME68X sensor data before processing
 * @param[in] outputs   : Processed BSEC BSEC output data
 * @param[in] bsec      : Instance of BSEC2 calling the callback
 */
void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec);

#endif