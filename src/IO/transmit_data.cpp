#include <io.h>
#include <string>
#include <base64.hpp>

static std::string to_base64(char *b_arr, size_t sz) {
  unsigned char base64_output[80] = {0};
  size_t encoded_len = encode_base64((unsigned char*) b_arr, sz, base64_output);
  return std::string((char*) base64_output, encoded_len);
}

void transmit_data(char *b_arr, size_t sz)
{
  auto encoded = to_base64(b_arr, sz);
  std::string str = "AT+SEND=2," + std::to_string(encoded.length()) + "," + encoded;
}
