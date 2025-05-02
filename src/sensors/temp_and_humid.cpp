#include <common_sensors.h>

Adafruit_HTU21DF temp_hud = Adafruit_HTU21DF();

bool verify_temp_and_humidity_temperature(float temp_and_humidity_temp_reading)
{
  if (temp_and_humidity_temp_reading < COMMON_LOWER_TEMP || temp_and_humidity_temp_reading > TEMP_HUMIDITY_UPPER_TEMP)
  {
    return false;
  }
  return true;
}

bool power_on_temp_and_humidity()
{
  if (temp_hud.begin() == false)
  {
    Serial.print("Temperature and humidity BEGIN function failed."); // print this
    return false;
  }

  if (!is_device_connected(TEMP_AND_HUMID_ADDRESS))
  {
    Serial.println("Temperature and humidity I2C check failed");
    return false;
  }

  if (!verify_temp_and_humidity_temperature(temp_hud.readTemperature()))
  {
    Serial.println("Temperature and humidity temperature check failed.");
    return false;
  }

  return true;
}

bool process_temp_and_humidity()
{
  float temperature = temp_hud.readTemperature();
  float humidity = temp_hud.readHumidity();
}