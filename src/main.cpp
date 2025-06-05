#include <state_manager.h>
#include <common_sensors.h>
#include <log.h>
#include <Arduino.h>
#include <string>
#include <state_manager.h>
#include <io.h>

void setup()
{
  initialize_all_components();
}

void loop()
{
  update_rocket_states(rocket);
  run_priority_sensor(rocket);
  log_current_time();
}
