#include <io.h>

bool write_and_transmit(enum sensors SID, float x) {
  unsigned char b_arr[1 + sizeof(float)] = {SID};
  memcpy(b_arr + 1, (unsigned char*) &x, sizeof(float));

  store_data(b_arr, sizeof(b_arr));
  transmit_data((char*)b_arr, sizeof(b_arr));

  return true;
}

bool write_and_transmit(enum sensors SID, serial &s) {
  s.ser.insert(s.ser.begin(), SID); // insert the header id in front of the data.
  
  store_data(&s.ser[0], s.ser.size());
  if(SID == IMUS) { // WE DO NOT WANT TO SEND ALL THE IMU'S DATA OVER RADIO! WE ONLY WANT TO SEND SOME OF THE DATA
    transmit_data(&s.ser[7 * sizeof(float)], 
                  8 * sizeof(float));
    return true;
  }
  transmit_data(&s.ser[0], s.ser.size());

  return true;
}
