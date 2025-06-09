
#include <Wire.h>
#include <Arduino.h>

bool power_on_lora_998()
{
    Serial1.begin(115200); // Initializing Serial communication

    Serial1.println("AT+MODE=0");
    delay(1000);
    Serial1.println("AT+NETWORKID=8");
    delay(1000);
    Serial1.println("AT+ADDRESS=1"); // transmitter address 1, ground station address 2
    delay(1000);
    Serial1.println("AT+BAND=915000000"); //setting frequency band to 915 MHz
    delay(1000);
    Serial1.println("AT+IPR=115200"); //setting baud rate 
    delay(1000);
    Serial1.println("Lora Transmitter Ready!");

    return true;
    //possibly switch to bool func, need to create error check 
}