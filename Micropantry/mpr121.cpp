#include "mpr121.h"

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

volatile bool touchInterruptFlag = false;

//  ISR triggered by the MPR121 INT pin
void IRAM_ATTR touchISR() {
  touchInterruptFlag = true;  // Set flag when interrupt occurs
}

void mpr121_init(void)
{
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5B)) {
    DEBUG_PRINTLN("MPR121 not found, check wiring?");
    while (1);
  }
  DEBUG_PRINTLN("MPR121 found!");
  cap.setThresholds(2, 3);

  pinMode(MPR121_INT_PIN, INPUT_PULLUP);  // Configure INT pin as input with pull-up
  attachInterrupt(digitalPinToInterrupt(MPR121_INT_PIN), touchISR, FALLING);  // Trigger on falling edge
}

void mpr121_run(void)
{
  // Check if the ISR flag is set (indicating a touch event)
  if (touchInterruptFlag) {
    touchInterruptFlag = false;  // Reset the flag

    // Get the currently touched pads
    currtouched = cap.touched();
    
    for (uint8_t i=0; i<12; i++) {
      // it if *is* touched and *wasnt* touched before, alert!
      if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
        DEBUG_PRINT(i); DEBUG_PRINTLN(" touched");
      }
      // if it *was* touched and now *isnt*, alert!
      if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
        DEBUG_PRINT(i); DEBUG_PRINTLN(" released");
      }
    }

    // reset our state
    lasttouched = currtouched;

    // comment out this line for detailed data from the sensor!
    return;
    
    // debugging info, what
    if (Serial) {
      Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
    }
    DEBUG_PRINT("Filt: ");
    for (uint8_t i=0; i<12; i++) {
      DEBUG_PRINT(cap.filteredData(i)); DEBUG_PRINT("\t");
    }
    DEBUG_PRINTLN();
    DEBUG_PRINT("Base: ");
    for (uint8_t i=0; i<12; i++) {
      DEBUG_PRINT(cap.baselineData(i)); DEBUG_PRINT("\t");
    }
    DEBUG_PRINTLN();
    
    // put a delay so it isn't overwhelming
    delay(100);
  }
}