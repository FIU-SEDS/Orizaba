#include <common_sensors.h>
#include <MS5611.h> // Barometer Library
#include <io.h>

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

// returns altitude from barometer in FEET
float read_altitude(float sea_level_pressure)
{
  float altitude;

  float pressure = (barometer.getPressurePascal()) / 100.0F; // conversion from pacals to hectopascals

  altitude = ATMOSPHERE_HEIGHT_METERS * (1.0F - pow(pressure / SEA_LEVEL_PRESSURE, PRESSURE_EXPONENT)); // formula for altitude
  altitude *= METERS_TO_FEET;                                                                           // converts from meters to feet

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
  double altitude = read_altitude(SEA_LEVEL_PRESSURE);
  float pressure = (barometer.getPressurePascal()) / 100.0F; // pascals converted to hectopascals

  serial barometer_data;

  barometer_data.serialize_float(altitude);
  barometer_data.serialize_float(pressure);

  write_and_transmit(BAROMETER, barometer_data);

  global_sensor_vals[ALTITUDE] = altitude;
  global_sensor_vals[PRESSURE] = pressure;

  return true;
}
