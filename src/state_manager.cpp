#include <state_manager.h>
#include <common_sensors.h>
#include <vector>
#include <cstdint>

#define ARRLEN(_x) ((sizeof(_x)) / (sizeof(_x[0])))

// Rocket State parameter thresholds
constexpr uint8_t G_FORCE_THRESHOLD = 3;                                                       // unit 1 G
constexpr float GRAVITY_TO_METERS_PER_SECOND = 9.80665;                                        // measured in m/s^2
constexpr float LINEAR_ACCEL_Z_THRESHOLD = (G_FORCE_THRESHOLD * GRAVITY_TO_METERS_PER_SECOND); // set to 3Gs; change the G_FORCE_THRESHOLD to set what amount of Gs the rocket will measure until it switches to BOOST
constexpr uint16_t ALTITUDE_THRESHOLD = 50;                                                    // unit FEET
rocket_state rocket = IDLE;                                                                    // tentative placement to not get any errors
constexpr uint8_t COAST_ITERARTIONS = 100;                                                     // this variables holds the amount of times that we will check that the rocket is sitll not accelerating to ensure that the rocket is in COAST stage
constexpr uint8_t APOGEE_ITERATIONS = 10;                                                      // this variable holds the amount of times we will check that the rockets previous altitude is greater than its current one to determine that the rocket has indeed reached APOGEE stage
constexpr uint16_t MAIN_PARACHUTE_THRESHOLD = 1000;                                            // unit FEET where main parachute is set to deploy

double previous_altitude = 0;    // this variable is set to keep track of a previous baseline altitude to refer to
uint8_t coast_sample_count = 0;  // used to verify that the coast counter reaches 100 to switch states
uint8_t apogee_sample_count = 0; // used to verify that the current altitude measurement in COAST stage is lower than the previous reading and repeated 5 times to ensure that the rocket is in apogee

bool initialize_all_components()
{
  if(!power_on_backup_IMU() || 
    !power_on_main_IMU() ||
    !power_on_backup_IMU() ||
    !power_on_real_time_clock() ||
    !power_on_magnetometer() ||
    !power_on_barometer() ||
    !power_on_temp_and_humidity() ||
    !power_on_GPS() ||
    !power_on_lora_998() ||
    !power_on_storage()
  ) {
    SCB_AIRCR = 0x05FA0004; // should reset teensy 4.1
  }
    else
  {
    rocket = IDLE; // changes rocket state to IDLE from INIT
  }

  return true;
}

static std::vector<bool(*)(void)> idle_prio    = {process_barometer, process_IMUs};
static std::vector<bool(*)(void)> boost_prio   = {process_barometer, process_IMUs};
static std::vector<bool(*)(void)> burnout_prio = {process_IMUs};
static std::vector<bool(*)(void)> coast_prio   = {process_barometer, process_IMUs};
static std::vector<bool(*)(void)> apogee_prio  = {process_barometer};
static std::vector<bool(*)(void)> descent_prio = {process_barometer, process_IMUs}; // process_GPS here
static std::vector<bool(*)(void)> landed_prio  = {}; // process_GPS here
static bool (*process_sensors[])(void) = {process_temp_and_humidity, process_barometer, process_IMUs, process_magnetometer};

bool run_priority_sensor(enum flight_state rs) {
  static enum flight_state old_state = IDLE;
  static size_t main_idx = 0;
  static size_t prio_idx = 0;
  static std::vector<bool(*)(void)> &current_prio = idle_prio;

  if(rs != old_state) {
    prio_idx = 0;
    switch(rs) {
      case INIT: // should be NO priority for INIT. This is just a holdover for there being an INIT rocket state, which there shouldn't be.
      case IDLE:
        current_prio = idle_prio;
        break;
      case BOOST:
        current_prio = boost_prio;
        break;
      case BURNOUT:
        current_prio = burnout_prio;
        break;
      case COAST:
        current_prio = coast_prio;
        break;
      case APOGEE:
        current_prio = apogee_prio;
        break;
      case DESCENT_MAIN:
      case DESCENT_DROGUE:
        current_prio = descent_prio;
        break;
      case LANDED:
        current_prio = landed_prio;
    }

  }

  if(main_idx % 2 != 0) {
    process_sensors[main_idx % ARRLEN(process_sensors)]();
    main_idx++;
  }
  else {
    current_prio[prio_idx]();
    if (current_prio.size() < prio_idx) prio_idx++;
    else main_idx++;
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
    if (global_sensor_vals[LINEAR_ACCELEARTION_Z] < LINEAR_ACCEL_Z_THRESHOLD || global_sensor_vals[ALTITUDE] > previous_altitude)
    {
      rocket = BURNOUT;
    }
    break;

  case BURNOUT:
    if (global_sensor_vals[LINEAR_ACCELEARTION_Z] < LINEAR_ACCEL_Z_THRESHOLD)
    {
      coast_sample_count++;
      if (coast_sample_count >= COAST_ITERARTIONS)
      {
        rocket = COAST;
        coast_sample_count = 0;
      }
    }
    else
    {
      coast_sample_count = 0; // reset counter if acceleration is still detected
    }
    break;

  case COAST:
    if (global_sensor_vals[ALTITUDE] < previous_altitude)
    {
      apogee_sample_count++;
      if (apogee_sample_count >= APOGEE_ITERATIONS)
      {
        rocket = APOGEE;
        apogee_sample_count = 0; // reset counter
      }
    }
    else
    {
      apogee_sample_count = 0; // reset if altitude is still increasing
    }
    break;

  case APOGEE:
    // write a function here to timestamp "Drogue parachute deployment"
    rocket = DESCENT_DROGUE;
    break;

  case DESCENT_DROGUE:
    if (global_sensor_vals[ALTITUDE] < MAIN_PARACHUTE_THRESHOLD)
    {
      rocket = DESCENT_MAIN;
    }
    break;

  case DESCENT_MAIN:
    if (global_sensor_vals[LINEAR_ACCELEARTION_Z] < LINEAR_ACCEL_Z_THRESHOLD || global_sensor_vals[ALTITUDE] < ALTITUDE_THRESHOLD)
    {
      rocket = LANDED;
    }
    break;

  case LANDED:
  // write a function here to timestamp "Rocket has landed"
    break;
  default:
    break;
  }
  previous_altitude = global_sensor_vals[ALTITUDE]; // updates previous altitude for the next iteration
}