#include "reedSwitch.h"

volatile bool reedSwitchStateChanged = false;
bool reedSwitchStatus = false; // True for CLOSED, False for OPEN

// ISR for the reed switch
void IRAM_ATTR reedSwitchStateChange()
{
  reedSwitchStateChanged = true; 
}

void reedSwitch_init(void)
{
    pinMode(REED_SWITCH_PIN, INPUT);
    
    attachInterrupt(digitalPinToInterrupt(REED_SWITCH_PIN), reedSwitchStateChange, CHANGE);
    Serial.println("Reed Switch (Door) Interrupt Setup Complete!");
}

void reedSwitch_run(void)
{
  if (reedSwitchStateChanged) {
    int switchState = digitalRead(REED_SWITCH_PIN); // Read current state of the reed switch

    // If switch is HIGH, the door is CLOSED
    if (switchState == HIGH) {
      reedSwitchStatus = true;  // Door is CLOSED
      Serial.println("Door CLOSED!");
    }

    // If switch is LOW, door is OPEN 
    else {
      reedSwitchStatus = false;  // Door is OPEN
      Serial.println("Door OPEN!");
    }

    reedSwitchStateChanged = false;  // Reset the flag
    delay(50);  // Optional debounce delay
  }
}

