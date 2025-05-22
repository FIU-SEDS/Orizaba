#include <state_manager.h>
#include <common_sensors.h>

rocket_state rocket = INIT; // tentative placement to not get any errors

bool initialize_all_components()
{

  if (!power_on_backup_IMU() ||
      !power_on_main_IMU() ||
      !power_on_backup_IMU() ||
      !power_on_real_time_clock() ||
      !power_on_magnetometer() ||
      !power_on_barometer() ||
      !power_on_temp_and_humidity() ||
      !power_on_GPS() ||
      !power_on_lora_998() ||
      !power_on_storage())
  {
    SCB_AIRCR = 0x05FA0004; // should reset teensy4.1
  }
  else
  {
    rocket = IDLE; // changes rocket state to IDLE from INIT
  }

  return true;
}

void check_states(rocket_state state)
{
  switch (state)
  {
  case INIT:
    initialize_all_components();
    break;

  case IDLE:
  // prioritize_sensors() overloaded function
  // checks parameters for rocket state and then switches enum value    
    break;

  case BOOST:

    break;

  case BURNOUT:

    break;

  case COAST:

    break;

  case

      default:
    break;
  }
}