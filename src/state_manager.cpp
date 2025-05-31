#include <state_manager.h>
#include <common_sensors.h>
#include <vector>
#include <cstdint>

#define ARRLEN(_x) ((sizeof(_x)) / (sizeof(_x[0])))

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
    SCB_AIRCR = 0x05FA0004; // should reset tensy4.1
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

  return true;
}
