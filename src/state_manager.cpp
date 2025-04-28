#include <state_manager.h>
#include <common_sensors.h>

bool initialize_all_sensors()
{
  // Global status arrays (defined in a .cpp file)
  bool critical_sensor_status[4] = {false};
  bool non_critical_sensor_status[3] = {false};

  
  // critical_sensor_status[BAROMETER] = power_on_barometer();
  // critical_sensor_status[MAIN_IMU] = power_on_main_imu();
  // critical_sensor_status[BACKUP_IMU] = power_on_backup_imu();
  // critical_sensor_status[GPS] = power_on_gps();

  non_critical_sensor_status[MAGNETOMETER] = power_on_magnetometer();
  // non_critical_sensor_status[TEMPHUD] = power_on_temphud();
  // non_critical_sensor_status[RTC] = power_on_real_time_clock();

  // Check if all critical sensors are initialized
  bool all_critical_initialized = true;
  for (int i = 0; i < 4; i++)
  {
    all_critical_initialized &= critical_sensor_status[i];
  }

  return all_critical_initialized;
}
