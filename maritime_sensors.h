/*
 * OndOcean Maritime Sensors Interface
 * Environmental sensors for maritime RemoteID module
 */

#ifndef MARITIME_SENSORS_H
#define MARITIME_SENSORS_H

#include <Arduino.h>
#include <Wire.h>

// Sensor I2C addresses
#define BME280_I2C_ADDR     0x76  // Environmental sensor
#define LSM6DS3_I2C_ADDR    0x6A  // IMU sensor
#define LIS3MDL_I2C_ADDR    0x1C  // Magnetometer

// Maritime sensor thresholds
#define TEMP_MIN_CELSIUS    -20.0f
#define TEMP_MAX_CELSIUS     60.0f
#define HUMIDITY_MAX_PCT     95.0f
#define PRESSURE_MIN_HPA    950.0f
#define PRESSURE_MAX_HPA   1050.0f

// Sensor data structure
struct MaritimeSensorData {
    float temperature_c;
    float humidity_pct;
    float pressure_hpa;
    float accel_x, accel_y, accel_z;
    float gyro_x, gyro_y, gyro_z;
    float mag_x, mag_y, mag_z;
    bool case_tamper_detected;
    float battery_voltage;
    uint32_t timestamp_ms;
};

// Function declarations
void setup_maritime_sensors();
bool read_maritime_sensors(MaritimeSensorData* data);
bool validate_maritime_environment(const MaritimeSensorData* data);
void maritime_sensor_calibration();
bool check_case_integrity();
float read_battery_voltage();

// Environmental monitoring
bool is_maritime_environment_safe();
void maritime_sensor_diagnostics();

#endif // MARITIME_SENSORS_H
