#define IO_USERNAME  "ishuuu"
#define IO_KEY       "aio_LSKT60s4aRsUYSJE7yW7PLOnr6nC"

/******************************* WIFI **************************************/
#define WIFI_SSID "Ekam"
#define WIFI_PASS "1234567890"

#include "AdafruitIO_WiFi.h"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

/************************** Example Starts Here *******************************/

#include <WiFi.h>  // ESP32 uses WiFi library instead of ESP8266WiFi

// set up the 'digital' feed
AdafruitIO_Feed *AssistiveCallButtons = io.feed("button_on");

void setup() {
  // set up serial monitor
  Serial.begin(115200);

  // wait for serial monitor to open
  while(!Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'digital' feed.
  AssistiveCallButtons->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  // recv. the assistive-call-buttons feed
  AssistiveCallButtons->get();

  // Initialize the built-in LED pin (ESP32 built-in LED is GPIO 2)
  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(LED_BUILTIN, LOW); // Ensure LED is off initially
}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
}

// this function is called whenever a 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {
  Serial.print("Received <- AssistiveCallButtons: ");
  Serial.println(data->toInt()); // Print the received value

  // Check the value received from Adafruit IO
  int buttonState = data->toInt();

  // Control the built-in LED based on the button state
  if (buttonState == 1) { // If button state is HIGH
    digitalWrite(LED_BUILTIN, HIGH); // Turn LED on
    Serial.println("LED turned ON");
  } else { // If button state is LOW
    digitalWrite(LED_BUILTIN, LOW); // Turn LED off
    Serial.println("LED turned OFF");
  }
}
