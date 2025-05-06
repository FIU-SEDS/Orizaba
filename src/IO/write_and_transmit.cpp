#include <io.h>
#include <cstdint>

bool write_and_transmit(enum sensors SID, float x) {
  unsigned char b_arr[1 + sizeof(x)] = {SID};
  unsigned char *ptr = (unsigned char*) &x;
  
  for(std::size_t i = 0; i < sizeof(x); i++) {
    b_arr[i + 1] = ptr[i];
  }

  store_data(b_arr, sizeof(b_arr));

  return false;
}

bool write_and_transmit(enum sensors SID, serial &s) {
  s.ser.insert(s.ser.begin(), SID);
  store_data(&s.ser[0], s.ser.size());
  return false;
}
