// #include <state_manager.h>
// #include <common_sensors.h>

#include <Arduino.h>
void setup()
{
  Serial.begin(115200);

    
    Serial.println("AT+MODE = 0");
    delay(1000);
    Serial.println("AT+ADDRESS = 1"); // transmitter address 1, ground station address 2 
    delay(1000);
    Serial.println("AT+BAND = 915000000");
    delay(1000);
    Serial.println("AT+IPR = 115200");
    delay(1000);
}
void loop() {
Serial.println("AT+SEND = 2,2,Hi");
delay(200);
}
