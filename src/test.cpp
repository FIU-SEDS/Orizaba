#include <Arduino.h>
#include <common_sensors.h>
#include <HardwareSerial.h>

constexpr uint8_t BUILTIN_LED = 13;
constexpr uint8_t RADIO_RST = 3;

void setup()
{
  Serial1.setRX(0);
  Serial1.setTX(1);
  Serial1.begin(115200);
  delay(500);

  power_on_lora_998();

  pinMode(BUILTIN_LED, OUTPUT);
  // pinMode(RADIO_RST, OUTPUT);

  // setup completed
  // digitalWrite(RADIO_RST, LOW);
}

void loop()
{
  digitalWrite(BUILTIN_LED, HIGH);
  // Serial1.println("AT+SEND=2,12,AGaWvkTA5sVH");
  delay(1000);
  Serial1.println("AT+SEND=2,5,HELLO");
  digitalWrite(BUILTIN_LED, LOW);
  // Serial1.println("AT+SEND=2,8,AmZmq0M=");
  delay(1000);
}