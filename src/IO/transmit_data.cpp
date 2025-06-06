#include <io.h>
#include <Arduino.h>

void transmit_data(char *b_arr, size_t sz)
{

  String data_string = "AT+SEND=2," + String(sz) + "," + String(b_arr);
 

  Serial.println(data_string);
}