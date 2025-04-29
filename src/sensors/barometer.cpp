#include <common_sensors.h>

MS5611 barometer;

bool verify_barometer_temperature(float barometer_temp_reading)
{
  if (barometer_temp_reading < COMMON_LOWER_TEMP || barometer_temp_reading > BAROMETER_UPPER_TEMP)
  {
    return false;
  }
  return true;
}

bool power_on_barometer()
{

  if (barometer.begin() == false)
  {
    Serial.println("Barometer BEGIN function failed.");
    return false;
  }

  if (!is_device_connected(BAROMETER_ADDRESS))
  {
    Serial.println("Barometer I2C check failed.");
    return false;
  }

  if (!verify_barometer_temperature(barometer.getTemperature()))
  {
    Serial.println("Barometer Temperature check failed.");
    return false;
  }

  return true;
}