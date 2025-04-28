#include <Wire.h> // Arduino I2C library
#include <common_sensors.h>

SFE_MMC5983MA magnetometer; // object

bool verify_magnetometer_temperature(int16_t mag_temp_reading)
{
  if (mag_temp_reading < MAGNETOMETER_LOWER_TEMP || mag_temp_reading > MAGNETOMETER_UPPER_TEMP)
  {
    return false;
  }
  return true;
}

bool power_on_magnetometer()
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

  if (!verify_magnetometer_temperature(magnetometer.getTemperature()))
  {
    Serial.println("Magentometer Temperature check failed.");
    return false;
  }

  return true; // once everything is checked and is ok then return true.
}