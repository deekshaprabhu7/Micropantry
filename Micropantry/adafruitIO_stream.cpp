#include "adafruitIO_stream.h"

unsigned long lastPublishTime = 0;  // Variable to store the last time data was sent
unsigned long publishInterval = 30000;  // Interval for sending data in milliseconds (10 seconds)

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, ssid, password);

// Setup the 'temperature' feed
AdafruitIO_Feed *temperatureFeed = io.feed("Temperature");
AdafruitIO_Feed *pressureFeed = io.feed("Pressure");
AdafruitIO_Feed *humidityFeed = io.feed("Humidity");
AdafruitIO_Feed *IAQFeed = io.feed("IAQ");
AdafruitIO_Feed *reedSwitchFeed = io.feed("door-switch");


void adafruitIO_init(void)
{
  DEBUG_PRINT("Connecting to Adafruit IO");
  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
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

  if (millis() - lastPublishTime > publishInterval)
  {
    lastPublishTime = millis();

    DEBUG_PRINT("Sending Data to Adafruit IO: ");
    temperatureFeed->save(temperature);
    pressureFeed->save(pressure);
    humidityFeed->save(humidity);
    IAQFeed->save(IAQ);
    reedSwitchFeed->save(reedSwitchStatus);
  }
}