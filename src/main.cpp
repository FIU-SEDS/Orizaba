#include <common_sensors.h>
#include <io.h>
#include <base64.hpp>
#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("--- simple base64 test ---");

  if (!power_on_lora_998())
  {
    Serial.println("ERROR");
    return;
  }

  Serial.println("RADIO INIT SUCCESS TEST STARTING");
}

void loop()
{
  uint8_t data[5]; // create 5 bytes of data
  data[0] = 1;     // test sensor's SENSOR_ID = 1

  float altitude_test_value = 154.423f; // float into raw data 4 bytes
  memcpy(&data[1], &altitude_test_value, 4);

  Serial.println("Raw Data: 5 bytes");
  for (int i = 0; i < 5; i++)
  {
    Serial.print("0x");
    if (data[i] < 16)
      Serial.print("0");
    Serial.print(data[i], HEX);
    Serial.println(" ");
  }

  unsigned char base64_output[20]; // Buffer for base64 output (5 bytes -> ~8 chars)
  unsigned int encoded_length = encode_base64(data, 5, base64_output);

  // Convert to String and trim to actual length
  String base64_data = String((char *)base64_output);
  base64_data = base64_data.substring(0, encoded_length);

  Serial.print("Regular float: ");
  Serial.println(altitude_test_value);

  Serial.print("Base-64 encoded: ");
  Serial.println(base64_data);

  transmit_data((char *)base64_data.c_str(), base64_data.length()); // c_str() allows std::string objects to be a C-styled string to match with function arguement

  delay(2000);
}