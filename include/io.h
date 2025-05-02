#ifndef IO_H
#define IO_H
#include <common_sensors.h>

bool store_data(unsigned char *arr, std::size_t sz);
bool write_and_transmit(enum sensors SID, double x);
bool write_and_transmit(enum sensors SID, int x);

#endif // IO_H