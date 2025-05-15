#include <common_sensors.h>
#include <MCP7940.h> // Real Time Clock Library

MCP7940_Class real_time_clock; // RTC object. class gives the object access to functions: begin(), deviceStatus(), etc

bool power_on_real_time_clock()//initialize and power on RTC
{
  if (real_time_clock.begin() == false)
  {
    Serial.println("Real time clock BEGIN function failed.");
    return false; //returns false if fails
  }

  if (!real_time_clock.deviceStatus()) // Turn on the oscillator if it's not already running
  {
    Serial.println("Oscillator is off, turning it on."); //print this if not running. crystalization
    bool deviceStatus = real_time_clock.deviceStart(); //Start oscillator and return state
    if (!deviceStatus)
    {
      Serial.println("Oscillator did not start."); //print if device fails 
      return false;
    }
  }

  #if RTC_ADJUST == 1
  real_time_clock.adjust(); 
  //  RTC is set to the date and time when the program was compiled
  // we set the time for the real time clock at compile time only ONCE. The clock is set up with
  // a battery as to never
  exit(0);
  #endif 

  if (!is_device_connected(REAL_TIME_CLOCK_ADDRESS))//RTCA is a macro that defines the devices i2c address
  {
    Serial.print("Real time clock I2C check failed.");//if it doesnt connect, print this
    return false;
  }

  return true;
}

// total duration in seconds 
double time_of_stages()
{
  DateTime now = real_time_clock.now(); //class and object

  //convert to seconds
  double Tseconds = now.hour() * 3600.0 + now.minute() * 60.0 + now.second();
  
}