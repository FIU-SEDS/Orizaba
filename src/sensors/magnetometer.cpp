#include <Wire.h> // Arduino I2C library
#include <common_sensors.h>

bool power_magnetometer()
{
  if (magnetometer.begin() == false)
  {
    Serial.print("Magnetometer Begin function failed."); // print this
    return false;
  }

  magnetometer.softReset(); // resest sensor for new values

  if (!is_device_connected(MAGNETOMETER_ADDRESS))
  {
    Serial.print("Magentometer I2C check failed.");
    return false;
  }

  if (!verify_magnetometer_temperature())
  {
    Serial.println("Magentometer Temperature check failed.");
    return false;
  }

  return true; // once everything is checked and is ok then return true.
}