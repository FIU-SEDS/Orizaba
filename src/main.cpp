#include <state_manager.h>
#include <common_sensors.h>

void setup()
{
  power_on_Lora();
 // initialize_all_sensors(); check
 void transmit_data(char *b_arr, size_t sz) {
  std::string str = "AT+SEND=2," + std::to_string(sz) + "," + std::string(b_arr, sz);
}
}
void loop() {

}
