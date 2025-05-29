#ifndef IO_H
#define IO_H
#include <common_sensors.h>
#include <vector>

struct serial {
  std::vector<unsigned char> ser;
  void serialize_float(float f);
};

bool store_data(unsigned char *arr, size_t sz);
bool write_and_transmit(enum sensors SID, float x);
void transmit_data(char *b_arr, size_t sz);

#endif // IO_H
