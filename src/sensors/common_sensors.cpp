#include <Wire.h>
#include <common_sensors.h>

bool isDeviceConnected(uint8_t address)
{
  Wire.beginTransmission(address);
  return (Wire.endTransmission() == 0); // Returns true if device responds
}

bool verify_magnetometer_temperature()
{
  int mag_temp = magnetometer.getTemperature(); // variable that stores the magnetometers die temperature (CELSIUS)
  if (mag_temp < MAGNETOMETER_LOWER_TEMP || mag_temp > MAGNETOMETER_UPPER_TEMP)
  {
    return false;
  }
  return true;
}