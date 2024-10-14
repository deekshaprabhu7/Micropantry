#include "adafruitIO_stream.h"

unsigned long lastPublishTime = 0;  // Variable to store the last time data was sent
unsigned long publishInterval = 10000;  // Interval for sending data in milliseconds (10 seconds)

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, ssid, password);

// Setup the 'temperature' feed
AdafruitIO_Feed *temperatureFeed = io.feed("temperature");

void adafruitIO_init(void)
{
  Serial.print("Connecting to Adafruit IO");
  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}


void adafruitIO_run()
{
  io.run();

  if (millis() - lastPublishTime > publishInterval)
  {
    lastPublishTime = millis();

    Serial.print("Sending temperature: ");
    Serial.println(temperature);
    temperatureFeed->save(temperature);
  }
}