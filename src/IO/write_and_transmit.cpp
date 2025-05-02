#include <io.h>
#include <cstdint>

bool write_and_transmit(enum sensors SID, double x) {
  unsigned char b_arr[1 + sizeof(x)] = {SID};
  unsigned char *ptr = (unsigned char*) &x;
  
  for(std::size_t i = 0; i < sizeof(double); i++) {
    b_arr[i + 1] = ptr[i];
  }

  store_data(b_arr, sizeof(b_arr));

  return false;
}

bool write_and_transmit(enum sensors SID, uint32_t x) {
  unsigned char b_arr[1 + sizeof(x)] = {SID};
  unsigned char *ptr = (unsigned char*) &x;
  
  for(std::size_t i = 0; i < sizeof(x); i++) {
    b_arr[i + 1] = ptr[i];
  }

  store_data(b_arr, sizeof(b_arr));

  return false;
}
