#include <common_sensors.h>

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

// components

ASM330LHHSensor backup_IMU(&DEV_I2C, ASM330LHH_I2C_ADD_L);

bool power_on_backup_IMU()
{
    if (backup_IMU.begin() != ASM330LHH_OK)
    {
        Serial.println("Backup IMU BEGIN function failed.");
        return false;
    }

    backup_IMU.Enable_X(); // enables accelerometer component on sensor
    backup_IMU.Enable_G(); // enables gyroscope component on sensor

    if (!is_device_connected(BACKUP_IMU_ADDRESS))
    {
        Serial.println("Backup IMU I2C check failed.");
        return false;
    }

    return true;
}