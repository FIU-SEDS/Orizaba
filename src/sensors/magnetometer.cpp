#include <Wire.h>//library
#include <SparkFun_MMC5983MA_Arduino_Library.h>//library
SFE_MMC5983MA magnetometer;//object
constexpr uint8_t MAGNETOMETER_ADDRESS = 0x30;//constant variable name. made data type
bool power_magnetometer(uint8_t address);//prototype function

void setup()
{
    Serial.begin(115200);//limit
    Wire.begin();//no limits 

}

void loop()//infinite loop
{

}

bool power_magnetometer(uint8_t address)//prototype function
{
    if(magnetometer.begin() == false)//if its = to false 
    {
        Serial.print("Magnetometer Begin function failed.");//print this
        return false;
    }

    magnetometer.softReset();//object. sets to 0
    Wire.beginTransmission(MAGNETOMETER_ADDRESS);//function
    if(Wire.endTransmission() != 0)// if 0 then success, if not then error
    {
        Serial.print("Magnetometer I2C failed.");
        return false;
    }

    uint8_t mag_temp = magnetometer.getTemperature();//declared 
    if(mag_temp < -40 || mag_temp > 105)
    {
        return false;
    }
    return true;//once everything is checked and is ok then return true. 
}