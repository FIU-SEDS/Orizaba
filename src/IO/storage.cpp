#include <SD.h>
#include <SPI.h>
//#include<SPIMemory.h> need to investigate why this fails to compile!

#include <common_sensors.h>

File sd_card;
//SPIFlash flash(4);

bool power_on_storage() {
  SD.begin(SD_CARD_MODULE_CS_PIN);

  if(!(sd_card = SD.open("sensorData.txt", FILE_WRITE))) {
    return true;
  }

  //flash.begin();

  return false;
}

bool store_data(unsigned char *arr, std::size_t sz) {
  sd_card.write(arr, sz);

  return false;
}
