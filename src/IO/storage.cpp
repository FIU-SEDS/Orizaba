#include <SD.h>
#include <SPI.h>
#include<SPIMemory.h>
#include <cstdint>
#include <vector>

#include <common_sensors.h>

File sd_card;
SPIFlash flash;

uint32_t last_addr;

void serial::serialize_float(float f)
{
  for(size_t i = 0; i < sizeof(f); i++) {
    vec.push_back(f[i]);
  }
}

bool power_on_storage() 
{
  SD.begin(SD_CARD_MODULE_CS_PIN);

  if(!(sd_card = SD.open("sensorData.txt", FILE_WRITE)))
    return false;

  flash.begin();

  // first 4 bytes of flash memory is reserved to indicate the last address written to
  // written for persistent storage in case of poor electrcal work. Blame Santiago.
  
  if((last_addr = flash.readULong(0, false)) == 0) // readULong internally stores the data as uint32_t
    last_addr = 4;
  
  return true;
}

bool store_data(unsigned char *arr, std::size_t sz) 
{
  sd_card.write(arr, sz);
  flash.writeByteArray(last_addr, arr, sz, false);
  last_addr += sz;
  flash.writeULong(0, last_addr, false);

  return true;
}
