#include "reedSwitch.h"

// Variables for the first reed switch
volatile bool reedSwitch1StateChanged = false;
bool reedSwitch1Status = false; // True for CLOSED, False for OPEN
std::queue<DoorEvent> door1Events; // Queue to track events for door 1

// Variables for the second reed switch
volatile bool reedSwitch2StateChanged = false;
bool reedSwitch2Status = false; // True for CLOSED, False for OPEN
std::queue<DoorEvent> door2Events; // Queue to track events for door 2

// ISR for the first reed switch
void IRAM_ATTR reedSwitch1StateChange() {
  reedSwitch1StateChanged = true;
}

// ISR for the second reed switch
void IRAM_ATTR reedSwitch2StateChange() {
  reedSwitch2StateChanged = true;
}

void reedSwitch_init(void) {
  // Initialize the first reed switch
  pinMode(REED_SWITCH1_PIN, INPUT_PULLUP); // Use pull-up resistor for stable input
  attachInterrupt(digitalPinToInterrupt(REED_SWITCH1_PIN), reedSwitch1StateChange, CHANGE);

  // Initialize the second reed switch
  pinMode(REED_SWITCH2_PIN, INPUT_PULLUP); // Use pull-up resistor for stable input
  attachInterrupt(digitalPinToInterrupt(REED_SWITCH2_PIN), reedSwitch2StateChange, CHANGE);

  DEBUG_PRINTLN("Reed Switches (Doors) Interrupt Setup Complete!");
}

void reedSwitch_run(void) {
  // Handle events for the first reed switch
  if (reedSwitch1StateChanged) {
    int switchState1 = digitalRead(REED_SWITCH1_PIN); // Read current state of the first reed switch
    unsigned long currentTime = millis();

    // Log the event for the first reed switch
    DoorEvent newEvent1;
    newEvent1.timestamp = currentTime;
    newEvent1.state = (switchState1 == LOW); // LOW = CLOSED, HIGH = OPEN
    door1Events.push(newEvent1);            // Add the event to the queue

    // Print for debugging
    if (switchState1 == LOW) {
      DEBUG_PRINTLN("Door 1 CLOSED!");  // LOW = CLOSED
      reedSwitch1Status = true;
    } else {
      DEBUG_PRINTLN("Door 1 OPEN!");    // HIGH = OPEN
      reedSwitch1Status = false;
    }

    reedSwitch1StateChanged = false; // Reset the flag
  }

  // Handle events for the second reed switch
  if (reedSwitch2StateChanged) {
    int switchState2 = digitalRead(REED_SWITCH2_PIN); // Read current state of the second reed switch
    unsigned long currentTime = millis();

    // Log the event for the second reed switch
    DoorEvent newEvent2;
    newEvent2.timestamp = currentTime;
    newEvent2.state = (switchState2 == LOW); // LOW = CLOSED, HIGH = OPEN
    door2Events.push(newEvent2);            // Add the event to the queue

    // Print for debugging
    if (switchState2 == LOW) {
      DEBUG_PRINTLN("Door 2 CLOSED!");  // LOW = CLOSED
      reedSwitch2Status = true;
    } else {
      DEBUG_PRINTLN("Door 2 OPEN!");    // HIGH = OPEN
      reedSwitch2Status = false;
    }

    reedSwitch2StateChanged = false; // Reset the flag
  }
}
