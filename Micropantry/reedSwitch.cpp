#include "reedSwitch.h"

volatile bool reedSwitchStateChanged = false;
bool reedSwitchStatus = false; // True for CLOSED, False for OPEN

std::queue<DoorEvent> doorEvents; // Queue to track door events

// ISR for the reed switch
void IRAM_ATTR reedSwitchStateChange() {
  reedSwitchStateChanged = true;
}

void reedSwitch_init(void) {
  pinMode(REED_SWITCH_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(REED_SWITCH_PIN), reedSwitchStateChange, CHANGE);
  DEBUG_PRINTLN("Reed Switch (Door) Interrupt Setup Complete!");
}

void reedSwitch_run(void) {
  if (reedSwitchStateChanged) {
    int switchState = digitalRead(REED_SWITCH_PIN); // Read current state of the reed switch
    unsigned long currentTime = millis();

    // Log the event
    DoorEvent newEvent;
    newEvent.timestamp = currentTime;
    newEvent.state = (switchState == HIGH); // HIGH = CLOSED, LOW = OPEN
    doorEvents.push(newEvent);             // Add the event to the queue

    // Print for debugging
    if (switchState == HIGH) {
      DEBUG_PRINTLN("Door CLOSED!");
      reedSwitchStatus = true;
    } else {
      DEBUG_PRINTLN("Door OPEN!");
      reedSwitchStatus = false;
    }

    reedSwitchStateChanged = false; // Reset the flag
  }
}

