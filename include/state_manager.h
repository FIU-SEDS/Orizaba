#pragma once
#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

// Enums for critical and non-critical sensors
enum critical_sensors
{
  BAROMETER,
  MAIN_IMU,
  BACKUP_IMU,
  GPS
};

enum non_critical_sensors
{
  MAGNETOMETER,
  TEMPHUD, // temperature & humidity sensor
  RTC      // real time clock
};

enum Axes
{
  X,
  Y,
  Z
};

// Flight states
enum flight_state
{
  IDLE,
  INIT,
  BOOST,
  BURNOUT,
  COAST,
  APOGEE,
  DESCENT_DROGUE,
  DESCENT_MAIN,
  LANDED
};

// State manager class

#endif