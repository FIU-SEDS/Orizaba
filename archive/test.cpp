#include <Arduino.h>
#include <common_sensors.h>
#include <HardwareSerial.h>

constexpr uint8_t BUILTIN_LED = 13;
constexpr uint8_t RADIO_RST = 3;

void setup()
{
  Serial1.begin(115200);
  delay(1000);

  power_on_lora_998();

  // pinMode(BUILTIN_LED, OUTPUT);
  // pinMode(RADIO_RST, OUTPUT);

  // setup completed
  // digitalWrite(RADIO_RST, LOW);
}

void loop()
{
  // Serial1.println("AT+SEND=2,12,AGaWvkTA5sVH");
  // digitalWrite(BUILTIN_LED, HIGH);
  // delay(500);
  Serial1.println("AT+SEND=0,5,HELLO"); // add explicit crlf
  delay(500);
  // digitalWrite(BUILTIN_LED, LOW);
  // Serial1.println("AT+SEND=2,8,AmZmq0M=");
}