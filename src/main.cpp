#include <state_manager.h>
#include <common_sensors.h>

void setup()
{
  power_on_Lora();
  if (!initialize_all_sensors())
  {
    Serial.println("[CRITICAL] One or more critical sensors failed. Halting...");
    
}
void loop() {

}
