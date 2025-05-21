#include <common_sensors.h>
#include <MCP7940.h> // Real Time Clock Library
#include <cstdint>
#include <string>
#include <arduino.h>

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
int get_epoch_seconds()
{
  DateTime now = real_time_clock.now(); //class and object

  return now.unixtime();//this function already counts seconds since 01/01/1970
}

//return type std string
std::string iteration_time_log() // this is called everytime we iterate the main loop. It logs the time in unix time with milliseconds appended to the end of it.
{
  static_assert(sizeof(uint32_t) == 4 && sizeof(uint16_t) == 2, "type is not the expected size.")
  DateTime now = real_time_clock.now();
  uint32_t epoch = now.unixtime();
  uint16_t millis = millis() % 1000;// for minimun space needed.
  
  std::string time((char*) &epoch, 4);
  time += std::string((char*) &millis, 2);

  return time; // time is a string where the first 4 bytes describe the epoch time, and the last 2
  // describe how many milliseconds 
}