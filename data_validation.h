/*
 * OndOcean Data Validation System
 * Robust validation for maritime RemoteID data
 * Implements safety checks and data integrity validation
 */

#ifndef DATA_VALIDATION_H
#define DATA_VALIDATION_H

#include <Arduino.h>
#include "maritime_sensors.h"
#include "opendroneid.h"

// Validation result codes
typedef enum {
    VALIDATION_OK = 0,
    VALIDATION_ERROR_NULL_POINTER,
    VALIDATION_ERROR_OUT_OF_RANGE,
    VALIDATION_ERROR_INVALID_FORMAT,
    VALIDATION_ERROR_MARITIME_UNSAFE,
    VALIDATION_ERROR_SENSOR_FAULT,
    VALIDATION_ERROR_COMMUNICATION,
    VALIDATION_ERROR_SECURITY_BREACH
} ValidationResult;

// Validation configuration
struct ValidationConfig {
    // Environmental limits for maritime operation
    float temp_min_celsius = -20.0f;
    float temp_max_celsius = 60.0f;
    float humidity_max_pct = 95.0f;
    float pressure_min_hpa = 950.0f;
    float pressure_max_hpa = 1050.0f;
    
    // Position validation
    double lat_min = -90.0;
    double lat_max = 90.0;
    double lon_min = -180.0;
    double lon_max = 180.0;
    float altitude_max_m = 120.0f;  // Maritime drone max altitude
    
    // Power management
    float battery_min_voltage = 3.0f;
    float battery_max_voltage = 4.2f;
    float battery_critical_voltage = 3.2f;
    
    // Communication timeouts
    uint32_t gnss_timeout_ms = 5000;
    uint32_t sensor_timeout_ms = 1000;
    uint32_t mqtt_timeout_ms = 10000;
};

// Global validation configuration
extern ValidationConfig validation_config;

// Core validation functions
ValidationResult validate_maritime_sensor_data(const MaritimeSensorData* data);
ValidationResult validate_gnss_position(double lat, double lon, float alt, float accuracy);
ValidationResult validate_odid_data(const ODID_UAS_Data* uas_data);
ValidationResult validate_battery_status(float voltage, float current = 0.0f);
ValidationResult validate_communication_health();

// Security validation
ValidationResult validate_case_integrity(bool case_closed, bool tamper_detected);
ValidationResult validate_firmware_integrity();
ValidationResult validate_mqtt_connection(const char* broker, int port);

// Data sanitization
bool sanitize_string(char* str, size_t max_len);
bool sanitize_coordinates(double* lat, double* lon);
bool sanitize_sensor_reading(float* value, float min_val, float max_val);

// Validation helpers
bool is_valid_latitude(double lat);
bool is_valid_longitude(double lon);
bool is_valid_altitude_maritime(float alt);
bool is_maritime_environment_safe(const MaritimeSensorData* data);
bool is_position_over_water(double lat, double lon);

// Error reporting
void log_validation_error(ValidationResult result, const char* context);
const char* validation_result_to_string(ValidationResult result);

// Validation statistics
struct ValidationStats {
    uint32_t total_validations;
    uint32_t successful_validations;
    uint32_t failed_validations;
    uint32_t sensor_errors;
    uint32_t position_errors;
    uint32_t security_errors;
    uint32_t last_error_timestamp;
    ValidationResult last_error_code;
};

extern ValidationStats validation_stats;

void reset_validation_stats();
void update_validation_stats(ValidationResult result);
void print_validation_stats();

#endif // DATA_VALIDATION_H
