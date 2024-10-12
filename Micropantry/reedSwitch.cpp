#include "reedSwitch.h"

const int switchPin = D0;

void reedSwitch_init(void)
{
    pinMode(switchPin, INPUT);
}

void reedSwitch_run(void)
{
  // Read the state of the switch 
  int switchState = digitalRead(switchPin);

  // Print the state to the Serial Monitor
  if (switchState == HIGH) {
    Serial.println("Switch is ON");
  } else {
    Serial.println("Switch is OFF");
  }
  delay(500);

}

