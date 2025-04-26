#pragma once
#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H
#include <cstdint>

constexpr uint8_t BACKUP_IMU_ADDRESS = 0x6A;
constexpr uint8_t MAIN_IMU_ADDRESS = 0x30;
constexpr uint8_t SD_CARD_MODULE_CS_PIN = 10;

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

#endif