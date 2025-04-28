#pragma once
#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H
#include <common_sensors.h>

enum critical_sensors
{
  BAROMETER,  // measures pressure and altitude
  MAIN_IMU,   // 9-axis Main IMU
  BACKUP_IMU, // 6-axis Backup IMU
  GPS         // GNSS GPS module
};

enum non_critical_sensors
{
  MAGNETOMETER, // measures magnetic field
  TEMPHUD,      // temperature & humidity sensor
  RTC           // real time clock
};

enum Axes
{
  X,
  Y,
  Z
};

#endif