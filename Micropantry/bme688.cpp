#include "bme688.h"

/* Create an object of the class Bsec2 */
Bsec2 envSensor;

float temperature = 0;
float pressure = 0;
float humidity = 0;
float IAQ = 0;

void bme688_init(void)
{
    /* Desired subscription list of BSEC2 outputs */
    bsecSensor sensorList[] = {
            BSEC_OUTPUT_IAQ,
            BSEC_OUTPUT_RAW_TEMPERATURE,
            BSEC_OUTPUT_RAW_PRESSURE,
            BSEC_OUTPUT_RAW_HUMIDITY,
            BSEC_OUTPUT_RAW_GAS,
            BSEC_OUTPUT_STABILIZATION_STATUS,
            BSEC_OUTPUT_RUN_IN_STATUS,
            BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
            BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
            BSEC_OUTPUT_STATIC_IAQ,
            BSEC_OUTPUT_CO2_EQUIVALENT,
            BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
            BSEC_OUTPUT_GAS_PERCENTAGE,
            BSEC_OUTPUT_COMPENSATED_GAS
    };

    pinMode(PANIC_LED, OUTPUT);

    /* Valid for boards with USB-COM. Wait until the port is open */
    while(!Serial) delay(10);

    /* Initialize the library and interfaces */
    if (!envSensor.begin(BME68X_I2C_ADDR_HIGH, Wire))
    {
        checkBsecStatus(envSensor);
    }
	
	/*
	 *	The default offset provided has been determined by testing the sensor in LP and ULP mode on application board 3.0
	 *	Please update the offset value after testing this on your product 
	 */
	if (SAMPLE_RATE == BSEC_SAMPLE_RATE_ULP)
	{
		envSensor.setTemperatureOffset(TEMP_OFFSET_ULP);
	}
	else if (SAMPLE_RATE == BSEC_SAMPLE_RATE_LP)
	{
		envSensor.setTemperatureOffset(TEMP_OFFSET_LP);
	}

    /* Subsribe to the desired BSEC2 outputs */
    if (!envSensor.updateSubscription(sensorList, ARRAY_LEN(sensorList), SAMPLE_RATE))
    {
        checkBsecStatus(envSensor);
    }

    /* Whenever new data is available call the newDataCallback function */
    envSensor.attachCallback(newDataCallback);

    DEBUG_PRINTLN("BSEC library version " + \
            String(envSensor.version.major) + "." \
            + String(envSensor.version.minor) + "." \
            + String(envSensor.version.major_bugfix) + "." \
            + String(envSensor.version.minor_bugfix));
}

void bme688_run(void)
{
    /* Call the run function often so that the library can 
     * check if it is time to read new data from the sensor  
     * and process it.
     */
    if (!envSensor.run())
    {
        checkBsecStatus(envSensor);
    }
}

void errLeds(void)
{
    while(1)
    {
        digitalWrite(PANIC_LED, HIGH);
        delay(ERROR_DUR);
        digitalWrite(PANIC_LED, LOW);
        delay(ERROR_DUR);
    }
}


void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec)
{
    if (!outputs.nOutputs)
    {
        return;
    }

    DEBUG_PRINTLN("BSEC outputs:\n\tTime stamp = " + String((int) (outputs.output[0].time_stamp / INT64_C(1000000))));
    for (uint8_t i = 0; i < outputs.nOutputs; i++)
    {
        const bsecData output  = outputs.output[i];
        switch (output.sensor_id)
        {
            case BSEC_OUTPUT_IAQ:
                IAQ = output.signal;
                DEBUG_PRINTLN("\tIAQ = " + String(output.signal));
                DEBUG_PRINTLN("\tIAQ accuracy = " + String((int) output.accuracy));
                break;
            case BSEC_OUTPUT_RAW_TEMPERATURE:
                temperature = output.signal;
                DEBUG_PRINTLN("\tTemperature = " + String(output.signal));
                break;
            case BSEC_OUTPUT_RAW_PRESSURE:
                pressure = output.signal;
                DEBUG_PRINTLN("\tPressure = " + String(output.signal));
                break;
            case BSEC_OUTPUT_RAW_HUMIDITY:
                humidity = output.signal;
                DEBUG_PRINTLN("\tHumidity = " + String(output.signal));
                break;
            case BSEC_OUTPUT_RAW_GAS:
                DEBUG_PRINTLN("\tGas resistance = " + String(output.signal));
                break;
            case BSEC_OUTPUT_STABILIZATION_STATUS:
                DEBUG_PRINTLN("\tStabilization status = " + String(output.signal));
                break;
            // case BSEC_OUTPUT_RUN_IN_STATUS:
            //     DEBUG_PRINTLN("\tRun in status = " + String(output.signal));
            //     break;
            // case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE:
            //     DEBUG_PRINTLN("\tCompensated temperature = " + String(output.signal));
            //     break;
            // case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY:
            //     DEBUG_PRINTLN("\tCompensated humidity = " + String(output.signal));
            //     break;
            // case BSEC_OUTPUT_STATIC_IAQ:
            //     DEBUG_PRINTLN("\tStatic IAQ = " + String(output.signal));
            //     break;
            // case BSEC_OUTPUT_CO2_EQUIVALENT:
            //     DEBUG_PRINTLN("\tCO2 Equivalent = " + String(output.signal));
            //     break;
            // case BSEC_OUTPUT_BREATH_VOC_EQUIVALENT:
            //     DEBUG_PRINTLN("\tbVOC equivalent = " + String(output.signal));
            //     break;
            // case BSEC_OUTPUT_GAS_PERCENTAGE:
            //     DEBUG_PRINTLN("\tGas percentage = " + String(output.signal));
            //     break;
            // case BSEC_OUTPUT_COMPENSATED_GAS:
            //     DEBUG_PRINTLN("\tCompensated gas = " + String(output.signal));
            //     break;
            default:
                break;
        }
    }
}

void checkBsecStatus(Bsec2 bsec)
{
    if (bsec.status < BSEC_OK)
    {
        DEBUG_PRINTLN("BSEC error code : " + String(bsec.status));
        errLeds(); /* Halt in case of failure */
    }
    else if (bsec.status > BSEC_OK)
    {
        DEBUG_PRINTLN("BSEC warning code : " + String(bsec.status));
    }

    if (bsec.sensor.status < BME68X_OK)
    {
        DEBUG_PRINTLN("BME68X error code : " + String(bsec.sensor.status));
        errLeds(); /* Halt in case of failure */
    }
    else if (bsec.sensor.status > BME68X_OK)
    {
        DEBUG_PRINTLN("BME68X warning code : " + String(bsec.sensor.status));
    }
}
