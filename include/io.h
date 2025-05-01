#include <cstdint>

enum sensors_id : uint8_t {
  BAROMETER_ID = 0,
  IMUS_ID,
  MAGNETOMETER_ID,
  REAL_TIME_CLOCK_ID,
  TEMP_AND_HUMID_ID
};

bool store_data(unsigned char *arr, std::size_t sz);
bool write_and_transmit(enum sensors_id SID, double x);
bool write_and_transmit(enum sensors_id SID, int x);
