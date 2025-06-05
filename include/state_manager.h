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
enum rocket_state : uint8_t
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

extern enum rocket_state rocket;

// State manager functions
bool initialize_all_components(); // if one sensor's power on function returns false then it will reset the teensy 4.1

void update_rocket_states(rocket_state state); // determines which state the rocket is in and prioritizes said sensor 

bool run_priority_sensor(rocket_state rs);

#endif
