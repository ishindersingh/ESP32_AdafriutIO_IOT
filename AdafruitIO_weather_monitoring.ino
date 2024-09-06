#define IO_USERNAME  "ishuuu"
#define IO_KEY       "aio_LSKT60s4aRsUYSJE7yW7PLOnr6nC"

/******************************* WIFI **************************************/
#define WIFI_SSID "Ekam"
#define WIFI_PASS "1234567890"

#include "AdafruitIO_WiFi.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>  // Library for BMP280 sensor

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Create an instance of the BMP280 sensor
Adafruit_BMP280 bmp; 

// set up feeds for temperature and pressure
AdafruitIO_Feed *temperature = io.feed("temp");
AdafruitIO_Feed *pressure = io.feed("pres");

void setup() {
  // start the serial connection
  Serial.begin(115200);
  while(!Serial);

  // connect to Adafruit IO
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.println(io.statusText());
  
  // Initialize BMP280 sensor
  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  // set up sampling for the sensor
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
  // io.run(); is required for all sketches.
  io.run();

  // Read temperature and pressure from the sensor
  float temp = bmp.readTemperature();
  float pres = bmp.readPressure() / 100.0F; // Pressure in hPa

  // Print sensor data to Serial Monitor
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" *C");
  
  Serial.print("Pressure = ");
  Serial.print(pres);
  Serial.println(" hPa");

  // Send temperature and pressure data to Adafruit IO
  temperature->save(temp);
  pressure->save(pres);

  // Delay before the next reading
  delay(2000); // 2 seconds delay
}
