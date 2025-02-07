#include "adafruitIO_stream.h"

unsigned long lastPublishTotalWeightTime = 0;  // Last publish time for total weight
unsigned long lastPublishSensorTime = 0;  // Last publish time for other sensor data
const unsigned long publishIntervalTotalWeight = 30000;  // 5 minutes interval
const unsigned long publishIntervalSensors = 900000;  // 15 minutes interval
 
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, ssid, password);

// Setup the 'temperature' feed
AdafruitIO_Feed *temperatureFeed = io.feed("Temperature");
// AdafruitIO_Feed *pressureFeed = io.feed("Pressure");
// AdafruitIO_Feed *humidityFeed = io.feed("Humidity");
// AdafruitIO_Feed *IAQFeed = io.feed("IAQ");
AdafruitIO_Feed *reedSwitch1Feed = io.feed("door-switch1");
AdafruitIO_Feed *reedSwitch1StatusFeed = io.feed("Door1 Status");
AdafruitIO_Feed *reedSwitch2Feed = io.feed("door-switch2");
AdafruitIO_Feed *reedSwitch2StatusFeed = io.feed("Door2 Status");
AdafruitIO_Feed *weightShelf1Feed = io.feed("Weight-Shelf1");
AdafruitIO_Feed *weightShelf2Feed = io.feed("Weight-Shelf2");
AdafruitIO_Feed *weightShelf3Feed = io.feed("Weight-Shelf3");
AdafruitIO_Feed *weightShelf4Feed = io.feed("Weight-Shelf4");
AdafruitIO_Feed *totalWeightFeed = io.feed("Total-Weight");

void adafruitIO_init(void)
{
  DEBUG_PRINT("Connecting to Adafruit IO");
  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    DEBUG_PRINT(".");
    delay(500);
  }

  // we are connected
  DEBUG_PRINTLN();
  DEBUG_PRINTLN(io.statusText());
}

void adafruitIO_run()
{
  io.run();
  // Send all door 1 events
while (!door1Events.empty()) {
  DoorEvent event = door1Events.front();
  door1Events.pop();

  String stateString = event.state ? "Door 1 CLOSED" : "Door 1 OPEN";
  DEBUG_PRINT("Sending Door 1 Event: ");
  DEBUG_PRINTLN(stateString + " at " + String(event.timestamp));

  reedSwitch1Feed->save(event.state);
  reedSwitch1StatusFeed->save(stateString);
}

// Send all door 2 events
while (!door2Events.empty()) {
  DoorEvent event = door2Events.front();
  door2Events.pop();

  String stateString = event.state ? "Door 2 CLOSED" : "Door 2 OPEN";
  DEBUG_PRINT("Sending Door 2 Event: ");
  DEBUG_PRINTLN(stateString + " at " + String(event.timestamp));

  reedSwitch2Feed->save(event.state);
  reedSwitch2StatusFeed->save(stateString);
}


  // // Send all door events in the queue
  // while (!doorEvents.empty()) {
  //   DoorEvent event = doorEvents.front();
  //   doorEvents.pop(); // Remove the event from the queue

  //   // Convert state to a string message
  //   String stateString = event.state ? "Door CLOSED" : "Door OPEN";

  //   // Print for debugging
  //   DEBUG_PRINT("Sending Door Event: ");
  //   DEBUG_PRINTLN(stateString + " at " + String(event.timestamp));

  //   // Send the message to Adafruit IO
  //   reedSwitchFeed->save(event.state);         // Send to Adafruit IO
  //   reedSwitchStatusFeed->save(stateString);  // Send the string to Adafruit IO feed
  // }

  unsigned long currentMillis = millis();

  // Check if it's time to send total weight data (every 5 seconds)
  if (currentMillis - lastPublishTotalWeightTime > publishIntervalTotalWeight) {
    lastPublishTotalWeightTime = currentMillis;

    DEBUG_PRINT("Sending Total Weight to Adafruit IO: ");
    totalWeightFeed->save(totalWeightToSend);
    DEBUG_PRINTLN(totalWeightToSend);
  }

  // Check if it's time to send other sensor data (every 30 seconds)
  if (currentMillis - lastPublishSensorTime > publishIntervalSensors) {
    lastPublishSensorTime = currentMillis;

    DEBUG_PRINT("Sending Sensor Data to Adafruit IO: ");
    // temperatureFeed->save(temperature);
    // pressureFeed->save(pressure);
    // humidityFeed->save(humidity);
    // IAQFeed->save(IAQ);
    reedSwitch1Feed->save(reedSwitch1Status);
    reedSwitch2Feed->save(reedSwitch2Status);
    weightShelf1Feed->save(currentWeight_shelf1);
    weightShelf2Feed->save(currentWeight_shelf2);
    weightShelf3Feed->save(currentWeight_shelf3);
    weightShelf4Feed->save(currentWeight_shelf4);
    DEBUG_PRINTLN("Sensor Data Sent");
  }
}
