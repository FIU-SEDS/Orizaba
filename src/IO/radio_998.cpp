
#include <Wire.h>
#include <Arduino.h>

void power_on_lora_998()
{
    Serial.begin(115200); // Initializing Serial communication

    Serial.println("AT+MODE=0");
    delay(1000);
    Serial.println("AT+ADDRESS=1"); // transmitter address 1, ground station address 2
    delay(1000);
    Serial.println("AT+BAND=915000000"); //setting frequency band to 915 MHz
    delay(1000);
    Serial.println("AT+IPR=115200"); //setting baud rate 
    delay(1000);
    Serial.println("Lora Transmitter Ready!");

    return true;
    //possibly switch to bool func, need to create error check 
}
