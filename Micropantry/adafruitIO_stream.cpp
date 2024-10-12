#include "adafruitIO_stream.h"

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

  // Simulate a temperature reading
  float temperature = 24.5;  // Replace this with actual sensor data
  
  // Send data to Adafruit IO
  Serial.print("Sending temperature: ");
  Serial.println(temperature);
  temperatureFeed->save(temperature);

  // Wait 10 seconds before sending next reading
  delay(10000);
}