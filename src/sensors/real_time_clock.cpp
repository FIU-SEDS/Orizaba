#include <common_sensors.h>
#include <MCP7940.h> // Real Time Clock Library

MCP7940_Class real_time_clock; // RTC object

bool power_on_real_time_clock()
{

  if (real_time_clock.begin() == false)
  {
    Serial.println("Real time clock BEGIN function failed.");
    return false;
  }

  if (!real_time_clock.deviceStatus()) // Turn on the oscillator if it's not already running
  {
    Serial.println("Oscillator is off, turning it on.");
    bool deviceStatus = real_time_clock.deviceStart(); // Start oscillator and return state
    if (!deviceStatus)
    {
      Serial.println("Oscillator did not start.");
      return false;
    }
  }

  real_time_clock.adjust(); //  RTC is set to the date and time when the program was compiled

  if (!is_device_connected(REAL_TIME_CLOCK_ADDRESS))
  {
    Serial.print("Real time clock I2C check failed.");
    return false;
  }

  return true;
}
