#include "reedSwitch.h"

volatile bool reedSwitchOpened = false;

// ISR for the reed switch
void IRAM_ATTR handleReedSwitchOpen()
{
  reedSwitchOpened = true; //Set flag when switch is open
}

void reedSwitch_init(void)
{
    pinMode(REED_SWITCH_PIN, INPUT);
    
    attachInterrupt(digitalPinToInterrupt(REED_SWITCH_PIN), handleReedSwitchOpen, RISING);
    Serial.println("Reed Switch (Door) Interrupt Setup Complete!");
}

void reedSwitch_run(void)
{
    if (reedSwitchOpened) {
    Serial.println("Door Opened!");
    reedSwitchOpened = false;  // Reset the flag

    delay(50);
  }
}

