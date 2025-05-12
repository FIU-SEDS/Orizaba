#include <UbxGpsConfig.h> // Library for configuring the u-blox GPS module
#include <UbxGpsNavPosllh.h> // Library for GPS navigation position
#include <common_sensors.h>
#include <io.h>


 UbxGpsNavPosllh posllh; // Create a GPS object

// Define constants for GPS
#define GPS_BAUDRATE 115200
#define GPS_ADDRESS 0x42  // Example I2C address for GPS (if applicable)

//Example GPS data
enum GPS_vals : uint8_t
{
    LATITUDE,
    LONGITUDE,
    ALTITUDE
};

// Create a GPS configuration object
    UbxGpsConfig<HardwareSerialIMXRT, usb_serial_class> *ubxGpsConfig = 
    new UbxGpsConfig<HardwareSerialIMXRT, usb_serial_class>(Serial1, Serial);

// Function to verify GPS connection
bool verify_gps_connection()
{
    // Attempt to restore GPS defaults
    ubxGpsConfig->setBaudrate(GPS_BAUDRATE);
    ubxGpsConfig->configure();

    // Check if the GPS is connected (example check, modify as needed)
    if (!is_device_connected(GPS_ADDRESS))
    {
        Serial.println("GPS I2C check failed.");
        return false;
    }

    Serial.println("GPS connection verified.");
    return true;
}

// Function to initialize the GPS module
bool power_on_gps()
{
    Serial.println("Initializing GPS...");

    // Set the GPS baud rate and message type
    ubxGpsConfig->setBaudrate(GPS_BAUDRATE);
    ubxGpsConfig->setMessage(UbxGpsConfigMessage::NavPossllh);
    ubxGpsConfig->setRate(100); // Set update rate to 100 ms (10 Hz)
    // Configure the GPS module
    ubxGpsConfig->configure();

    // Verify GPS connection
    if (!verify_gps_connection())
    {
        Serial.println("GPS initialization failed.");
        return false;
    }

    Serial.println("GPS initialized successfully.");
    return true;
}

// Function to process GPS data
bool process_gps()
{
    // Feed incoming bytes from the GPS to the POSLLH parser
    while (Serial1.available()) {
        byte c = Serial1.read();
        posllh.update(c); 
    }

    // If a full valid POSLLH message has been parsed
    if (posllh.ready()) {

        // Extract and scale the position values
        float latitude = posllh.getLatitude() / 1e7;   // Convert from 1e-7 degrees to degrees
        float longitude = posllh.getLongitude() / 1e7; 
        float altitude = posllh.getAltitude() / 1000.0; // Convert mm to meters

        // Store and/or transmit the GPS data
        global_sensor_vals[LATITUDE] = latitude;
        global_sensor_vals[LONGITUDE] = longitude;
        global_sensor_vals[ALTITUDE] = altitude;

        Serial.print("Lat: "); Serial.println(latitude, 7); 

        Serial.print("Long: "); Serial.println(longitude, 7); 

        Serial.print("Alt: "); Serial.println(altitude); Serial.println(" m");


        posllh.clear(); // Clear the ready flag for next message

        return true;
    }

    return false; // No new GPS data this cycle
}