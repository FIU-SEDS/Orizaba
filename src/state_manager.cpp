#include <state_manager.h>
#include <common_sensors.h>

bool initialize_all_sensors()
{

  if(!power_on_backup_IMU() || 
    !power_on_main_IMU() ||
    !power_on_backup_IMU() ||
    !power_on_real_time_clock() ||
    !power_on_magnetometer() ||
    !power_on_barometer() ||
    !power_on_temp_and_humidity() ||
    !power_on_GPS()
  ) {
    SCB_AIRCR = 0x05FA0004;
  }

  return true;
}
