#include <common_sensors.h>
#include <MS5611.h> // Barometer Library

MS5611 barometer; // Barometer MS5611 object

/// Atlitude calculations constants
constexpr uint32_t ATMOSPHERE_HEIGHT_METERS = 44330; // Height where pressure approaches zero
constexpr float PRESSURE_EXPONENT = 0.1903;          // 1/5.255, related to atmospheric lapse rate
constexpr float SEA_LEVEL_PRESSURE = 1014.14;        // 845 hPa (Midland, Texas Estimate) 1014.4 hPa (Miami, Florida Estimate)
constexpr float METERS_TO_FEET = 3.280839895;        // Conversion factor from meters to feet

bool verify_barometer_temperature(float barometer_temp_reading)
{
  if (barometer_temp_reading < COMMON_LOWER_TEMP || barometer_temp_reading > COMMON_UPPER_TEMP)
  {
    return false;
  }
  return true;
}

float read_altitude(float sea_level_pressure)
{
  float altitude;

  float pressure = barometer.getPressurePascal();

  altitude = ATMOSPHERE_HEIGHT_METERS * (1.0 - pow(pressure / SEA_LEVEL_PRESSURE, PRESSURE_EXPONENT)); // formula for altitude
  altitude *= METERS_TO_FEET;                                                                          // converts from meters to feet

  return altitude;
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

bool process_barometer()
{
}