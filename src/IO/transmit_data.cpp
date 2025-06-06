#include <io.h>
#include <Arduino.h>

void transmit_data(char *b_arr, size_t sz)
{
  Serial.print("AT+SEND=2,");
  Serial.print(sz);
  Serial.print(",");
  Serial.write((uint8_t*)b_arr, sz);
  Serial.println();
}