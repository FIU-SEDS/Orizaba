#include <common_sensors.h>

Adafruit_HTU21DF temp_hud = Adafruit_HTU21DF();

bool power_on_temp_and_humid()
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

  return true;
}
