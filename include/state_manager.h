#pragma once
#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H
#include <cstdint>

enum axes : uint8_t
{
  X,
  Y,
  Z
};

// Flight states
enum flight_state : uint8_t
{
  INIT,
  IDLE,
  BOOST,
  BURNOUT,
  COAST,
  APOGEE,
  DESCENT_DROGUE,
  DESCENT_MAIN,
  LANDED
};

// State manager functions
bool initialize_all_sensors();

#endif