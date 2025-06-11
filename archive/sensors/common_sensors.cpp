#include <common_sensors.h>

float global_sensor_vals[SENSOR_READOUTS_AMOUNT] = {0};

bool is_device_connected(uint8_t address)
{
  Wire.beginTransmission(address);
  return (Wire.endTransmission() == 0); // Returns true if device responds
}
