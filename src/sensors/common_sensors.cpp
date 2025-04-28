#include <Wire.h>
#include <common_sensors.h>

bool isDeviceConnected(uint8_t address)
{
  Wire.beginTransmission(address);
  return (Wire.endTransmission() == 0); // Returns true if device responds
}

bool verify_magnetometer_temperature(int16_t mag_temp_reading)
{
  if (mag_temp_reading < MAGNETOMETER_LOWER_TEMP || mag_temp_reading > MAGNETOMETER_UPPER_TEMP)
  {
    return false;
  }
  return true;
}