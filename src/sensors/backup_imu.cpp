//includes 
#include <ASM330LHHSensor.h>

#ifdef ARDUINO_SAM_DUE
#define DEV_I2C Wire1
#elif defined(ARDUINO_ARCH_STM32)
#define DEV_I2C Wire 
#elif defined(ARDUINO_ARCH_AVR)
#define DEV_I2C Wire 
#else
#define DEV_I2C Wire 
#endif
#define SerialPort Serial 

//components 
ASM330LHHSensor AccGyr(&DEV_I2C, ASM330LHH_I2C_ADD_L);

void setup() {

    SerialPort.begin(9600);

    DEV_I2C.begin();

    AccGyr.begin();
    AccGyr.Enable_X();
    AccGyr.Enable_G();

    //add Wire library and Sensor library #include 
    
}
