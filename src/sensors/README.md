The structure of the src directory is as follows:
**DISCLAIMER**: Header files are stored in the include directory

|--src
| |-- main.cpp
| |-- state_manager.cpp
| |
| |
| |--sensors
| | |- gps.cpp
| | |- backup_imu.cpp
| | |- main_imu.cpp
| | |- barometer.cpp
| | |- temphumidty.cpp
| | |- magnetometer.cpp
| | |- real_time_clock.cpp
| | |- common_sensors.cpp
| |  
| |--IO (input/output)
| | |- storage.cpp
| | |- radio_998.cpp
| | |- radio_698.cpp
