#include <io.h>
#include <string>

void transmit_data(char *b_arr, size_t sz) {
    std::string str = "AT+SEND=2," + std::to_string(sz) + "," + std::string(b_arr, sz);
  }