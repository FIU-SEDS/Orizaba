#include <state_manager.h>
#include <common_sensors.h>
#include <log.h>
#include <Arduino.h>
#include <string>

static enum flight_state fs = IDLE;

void setup() {
  initialize_all_components();
  SCB_AIRCR = 0x05FA0004; // should reset tensy4.1
}

void loop()
{
  check_states
  SCB_AIRCR = 0x05FA0004; // should reset tensy4.1
}
