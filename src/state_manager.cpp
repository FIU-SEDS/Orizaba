#include <state_manager.h>
#include <common_sensors.h>

// Rocket State parameter thresholds
constexpr uint8_t G_FORCE_THRESHOLD = 3;                                                       // unit 1 G
constexpr float GRAVITY_TO_METERS_PER_SECOND = 9.80665;                                        // measured in m/s^2
constexpr float LINEAR_ACCEL_Z_THRESHOLD = (G_FORCE_THRESHOLD * GRAVITY_TO_METERS_PER_SECOND); // set to 3Gs; change the G_FORCE_THRESHOLD to set what amount of Gs the rocket will measure until it switches to BOOST
constexpr uint16_t ALTITUDE_THRESHOLD = 50;                                                    // unit FEET
rocket_state rocket = IDLE;                                                                    // tentative placement to not get any errors

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

void update_rocket_states(rocket_state state)
{
  switch (state)
  {
  case IDLE:
    if (global_sensor_vals[LINEAR_ACCELEARTION_Z] > LINEAR_ACCEL_Z_THRESHOLD || global_sensor_vals[ALTITUDE] > ALTITUDE_THRESHOLD)
    {
      rocket = BOOST;
    }
    break;

  case BOOST:

    break;

  case BURNOUT:

    break;

  case COAST:

    break;

  default:
    break;
  }
}