#include <common_sensors.h>
#include <io.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h> // Magnetometer Sensor Library

SFE_MMC5983MA magnetometer; // Magnetometer MMC5983MA object

// Verifies that the magnetometer internal temperature is between -40 and 105 degress Celsius
bool verify_magnetometer_temperature(int16_t mag_temp_reading)
{
  if (mag_temp_reading < COMMON_LOWER_TEMP || mag_temp_reading > MAGNETOMETER_UPPER_TEMP)
  {
    return false;
  }
  return true;
}

bool power_on_magnetometer()
{
  if (magnetometer.begin() == false)
  {
    Serial.print("Magnetometer BEGIN function failed."); // print this
    return false;
  }

  magnetometer.softReset(); // resest sensor for new values

  if (!is_device_connected(MAGNETOMETER_ADDRESS))
  {
    Serial.print("Magentometer I2C check failed.");
    return false;
  }

  if (!verify_magnetometer_temperature(magnetometer.getTemperature()))
  {
    Serial.println("Magentometer Temperature check failed.");
    return false;
  }

  return true; // once everything is checked and is ok then return true.
}

double get_heading() // function for direction in degrees
{
  // heading calculated(angle/compass direction)
  double Degrees = 0;

  // unsigned 32-bit int to store raw magnetometer readings for X,Y,Z axises.
  uint32_t rawValueX = 0; // X,Y,Z values
  uint32_t rawValueY = 0; // double check if unit32 or uint18!!!
  uint32_t rawValueZ = 0;
  // variables to hold scaled(normalized) versions of X,Y,Z values.
  double scaledX = 0;
  double scaledY = 0;
  double scaledZ = 0;

  // Read all three channels simultaneously
  // Calls a function on the magnetometer object to fill in the rawX, Y, Z values.
  // passes the addresses of rawValueX,Y,Z so the function can update them directly.
  magnetometer.getMeasurementXYZ(&rawValueX, &rawValueY, &rawValueZ);

  // Converts the rawX value to double, subtracts 131072.0, divides by 131072.0.
  // This scales the raw value into a range roughly between -1 and +1.
  //(Likely the raw range is 0 to 262144, centered(midpoint) at 131072)
  // range from 0 to 262143 because uint18(literally the definition). magnetic field max neg field is 0 or -1 and max pos field is 262143 or +1
  // this range includes neg and pos magnetic fields so the true 0 magnetic field is in the midpoint which is 131072.0(AKA 2^17)
  scaledX = (double)rawValueX - 131072.0; // 131072.0 is the midpoint
  scaledX /= 131072.0;                    // divide by midpoint and new scaledX value is given.
  scaledY = (double)rawValueY - 131072.0;
  scaledY /= 131072.0;
  scaledZ = (double)rawValueZ - 131072.0;
  scaledZ /= 131072.0;

  // atan2 returns a value between +PI and -PI
  // Converts to degrees
  // uses atan2(y,x) to calculate angle in radians from the x and y magnetometer axes.
  // 0 - scaledY reverses the Y-axis (likely due to coordinate system adjustment).
  // atan2 gives the angle between the vector (X, Y) and the positive X-axis.
  Degrees = atan2(scaledX, 0 - scaledY);

  // atan2 returns a value between +PI and -PI
  // convert from radians to a range from -1 to +1
  Degrees /= PI;
  Degrees *= 180; // convert to degrees
  Degrees += 180; // shift to range [0,360]
  // now suitable as compass direction
  
  return Degrees;
}

bool process_magnemeter() {
  double heading = get_heading();
  write_and_transmit(MAGNETOMETER, heading);
  global_sensor_vals[HEADING] = heading;

  return true;
}
