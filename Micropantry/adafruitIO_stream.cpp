#include "adafruitIO_stream.h"

unsigned long lastPublishTotalWeightTime = 0;  // Last publish time for total weight
unsigned long lastPublishSensorTime = 0;  // Last publish time for other sensor data
const unsigned long publishIntervalTotalWeight = 5000;  // 5 seconds interval
const unsigned long publishIntervalSensors = 60000;  // 30 seconds interval

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, ssid, password);

// Setup the 'temperature' feed
AdafruitIO_Feed *temperatureFeed = io.feed("Temperature");
AdafruitIO_Feed *pressureFeed = io.feed("Pressure");
AdafruitIO_Feed *humidityFeed = io.feed("Humidity");
// AdafruitIO_Feed *IAQFeed = io.feed("IAQ");
AdafruitIO_Feed *reedSwitchFeed = io.feed("door-switch");
AdafruitIO_Feed *reedSwitchStatusFeed = io.feed("Door Status");
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

  // Send all door events in the queue
  while (!doorEvents.empty()) {
    DoorEvent event = doorEvents.front();
    doorEvents.pop(); // Remove the event from the queue

    // Convert state to a string message
    String stateString = event.state ? "Door CLOSED" : "Door OPEN";

    // Print for debugging
    DEBUG_PRINT("Sending Door Event: ");
    DEBUG_PRINTLN(stateString + " at " + String(event.timestamp));

    // Send the message to Adafruit IO
    reedSwitchFeed->save(event.state);         // Send to Adafruit IO
    reedSwitchStatusFeed->save(stateString);  // Send the string to Adafruit IO feed
  }

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
    temperatureFeed->save(temperature);
    pressureFeed->save(pressure);
    humidityFeed->save(humidity);
    // IAQFeed->save(IAQ);
    reedSwitchFeed->save(reedSwitchStatus);
    weightShelf1Feed->save(currentWeight_shelf1);
    weightShelf2Feed->save(currentWeight_shelf2);
    weightShelf3Feed->save(currentWeight_shelf3);
    weightShelf4Feed->save(currentWeight_shelf4);
    DEBUG_PRINTLN("Sensor Data Sent");
  }
}
