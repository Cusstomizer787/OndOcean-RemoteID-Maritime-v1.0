/*
 * OndOcean Data Validation System Implementation
 * Robust validation for maritime RemoteID data
 */

#include "data_validation.h"
#include "ondocean_logger.h"
#include <math.h>
#include <string.h>

// Global validation configuration
ValidationConfig validation_config;
ValidationStats validation_stats = {0};

// Validation result strings for debugging
const char* validation_result_strings[] = {
    "OK",
    "NULL_POINTER",
    "OUT_OF_RANGE", 
    "INVALID_FORMAT",
    "MARITIME_UNSAFE",
    "SENSOR_FAULT",
    "COMMUNICATION_ERROR",
    "SECURITY_BREACH"
};

const char* validation_result_to_string(ValidationResult result) {
    if (result < sizeof(validation_result_strings) / sizeof(validation_result_strings[0])) {
        return validation_result_strings[result];
    }
    return "UNKNOWN_ERROR";
}

void log_validation_error(ValidationResult result, const char* context) {
    LOG_VALIDATION_ERROR("%s: %s", 
                         context ? context : "Unknown", 
                         validation_result_to_string(result));
    
    // Update statistics
    validation_stats.last_error_code = result;
    validation_stats.last_error_timestamp = millis();
}

ValidationResult validate_maritime_sensor_data(const MaritimeSensorData* data) {
    validation_stats.total_validations++;
    
    // Null pointer check
    if (!data) {
        log_validation_error(VALIDATION_ERROR_NULL_POINTER, "sensor_data");
        validation_stats.failed_validations++;
        validation_stats.sensor_errors++;
        return VALIDATION_ERROR_NULL_POINTER;
    }
    
    // Temperature validation
    if (data->temperature_c < validation_config.temp_min_celsius || 
        data->temperature_c > validation_config.temp_max_celsius) {
        log_validation_error(VALIDATION_ERROR_OUT_OF_RANGE, "temperature");
        validation_stats.failed_validations++;
        validation_stats.sensor_errors++;
        return VALIDATION_ERROR_OUT_OF_RANGE;
    }
    
    // Humidity validation
    if (data->humidity_pct < 0.0f || data->humidity_pct > validation_config.humidity_max_pct) {
        log_validation_error(VALIDATION_ERROR_OUT_OF_RANGE, "humidity");
        validation_stats.failed_validations++;
        validation_stats.sensor_errors++;
        return VALIDATION_ERROR_OUT_OF_RANGE;
    }
    
    // Pressure validation (maritime environment check)
    if (data->pressure_hpa < validation_config.pressure_min_hpa || 
        data->pressure_hpa > validation_config.pressure_max_hpa) {
        log_validation_error(VALIDATION_ERROR_MARITIME_UNSAFE, "pressure");
        validation_stats.failed_validations++;
        validation_stats.sensor_errors++;
        return VALIDATION_ERROR_MARITIME_UNSAFE;
    }
    
    // IMU data validation (check for reasonable values)
    float accel_magnitude = sqrt(data->accel_x * data->accel_x + 
                                data->accel_y * data->accel_y + 
                                data->accel_z * data->accel_z);
    if (accel_magnitude < 0.5f || accel_magnitude > 20.0f) {  // 0.5g to 20g range
        log_validation_error(VALIDATION_ERROR_SENSOR_FAULT, "accelerometer");
        validation_stats.failed_validations++;
        validation_stats.sensor_errors++;
        return VALIDATION_ERROR_SENSOR_FAULT;
    }
    
    // Timestamp validation (data freshness)
    uint32_t current_time = millis();
    if (data->timestamp_ms > current_time || 
        (current_time - data->timestamp_ms) > validation_config.sensor_timeout_ms) {
        log_validation_error(VALIDATION_ERROR_COMMUNICATION, "sensor_timestamp");
        validation_stats.failed_validations++;
        validation_stats.sensor_errors++;
        return VALIDATION_ERROR_COMMUNICATION;
    }
    
    validation_stats.successful_validations++;
    return VALIDATION_OK;
}

ValidationResult validate_gnss_position(double lat, double lon, float alt, float accuracy) {
    validation_stats.total_validations++;
    
    // Latitude validation
    if (!is_valid_latitude(lat)) {
        log_validation_error(VALIDATION_ERROR_OUT_OF_RANGE, "latitude");
        validation_stats.failed_validations++;
        validation_stats.position_errors++;
        return VALIDATION_ERROR_OUT_OF_RANGE;
    }
    
    // Longitude validation
    if (!is_valid_longitude(lon)) {
        log_validation_error(VALIDATION_ERROR_OUT_OF_RANGE, "longitude");
        validation_stats.failed_validations++;
        validation_stats.position_errors++;
        return VALIDATION_ERROR_OUT_OF_RANGE;
    }
    
    // Maritime altitude validation
    if (!is_valid_altitude_maritime(alt)) {
        log_validation_error(VALIDATION_ERROR_MARITIME_UNSAFE, "altitude");
        validation_stats.failed_validations++;
        validation_stats.position_errors++;
        return VALIDATION_ERROR_MARITIME_UNSAFE;
    }
    
    // Accuracy validation (maritime operations require good accuracy)
    if (accuracy > 10.0f) {  // 10 meter accuracy threshold
        log_validation_error(VALIDATION_ERROR_OUT_OF_RANGE, "gnss_accuracy");
        validation_stats.failed_validations++;
        validation_stats.position_errors++;
        return VALIDATION_ERROR_OUT_OF_RANGE;
    }
    
    validation_stats.successful_validations++;
    return VALIDATION_OK;
}

ValidationResult validate_battery_status(float voltage, float current) {
    validation_stats.total_validations++;
    
    // Voltage range check
    if (voltage < validation_config.battery_min_voltage || 
        voltage > validation_config.battery_max_voltage) {
        log_validation_error(VALIDATION_ERROR_OUT_OF_RANGE, "battery_voltage");
        validation_stats.failed_validations++;
        return VALIDATION_ERROR_OUT_OF_RANGE;
    }
    
    // Critical battery warning
    if (voltage < validation_config.battery_critical_voltage) {
        log_validation_error(VALIDATION_ERROR_MARITIME_UNSAFE, "battery_critical");
        validation_stats.failed_validations++;
        return VALIDATION_ERROR_MARITIME_UNSAFE;
    }
    
    validation_stats.successful_validations++;
    return VALIDATION_OK;
}

ValidationResult validate_case_integrity(bool case_closed, bool tamper_detected) {
    validation_stats.total_validations++;
    
    // Security breach detection
    if (tamper_detected) {
        log_validation_error(VALIDATION_ERROR_SECURITY_BREACH, "case_tamper");
        validation_stats.failed_validations++;
        validation_stats.security_errors++;
        return VALIDATION_ERROR_SECURITY_BREACH;
    }
    
    // Case should be closed for maritime operation
    if (!case_closed) {
        log_validation_error(VALIDATION_ERROR_MARITIME_UNSAFE, "case_open");
        validation_stats.failed_validations++;
        validation_stats.security_errors++;
        return VALIDATION_ERROR_MARITIME_UNSAFE;
    }
    
    validation_stats.successful_validations++;
    return VALIDATION_OK;
}

// Helper functions
bool is_valid_latitude(double lat) {
    return (lat >= validation_config.lat_min && lat <= validation_config.lat_max);
}

bool is_valid_longitude(double lon) {
    return (lon >= validation_config.lon_min && lon <= validation_config.lon_max);
}

bool is_valid_altitude_maritime(float alt) {
    return (alt >= 0.0f && alt <= validation_config.altitude_max_m);
}

bool is_maritime_environment_safe(const MaritimeSensorData* data) {
    if (!data) return false;
    
    // Check for extreme weather conditions
    if (data->temperature_c < -10.0f || data->temperature_c > 50.0f) {
        return false;
    }
    
    // Check for high humidity (potential for condensation)
    if (data->humidity_pct > 90.0f) {
        return false;
    }
    
    // Check for low pressure (storm conditions)
    if (data->pressure_hpa < 980.0f) {
        return false;
    }
    
    return true;
}

bool sanitize_string(char* str, size_t max_len) {
    if (!str) return false;
    
    // Ensure null termination
    str[max_len - 1] = '\0';
    
    // Remove non-printable characters
    for (size_t i = 0; i < max_len && str[i] != '\0'; i++) {
        if (str[i] < 32 || str[i] > 126) {
            str[i] = '?';  // Replace with placeholder
        }
    }
    
    return true;
}

bool sanitize_coordinates(double* lat, double* lon) {
    if (!lat || !lon) return false;
    
    // Clamp latitude to valid range
    if (*lat < -90.0) *lat = -90.0;
    if (*lat > 90.0) *lat = 90.0;
    
    // Normalize longitude to [-180, 180] range
    while (*lon < -180.0) *lon += 360.0;
    while (*lon > 180.0) *lon -= 360.0;
    
    return true;
}

bool sanitize_sensor_reading(float* value, float min_val, float max_val) {
    if (!value) return false;
    
    // Check for NaN or infinite values
    if (isnan(*value) || isinf(*value)) {
        *value = (min_val + max_val) / 2.0f;  // Set to middle value
        return false;
    }
    
    // Clamp to valid range
    if (*value < min_val) *value = min_val;
    if (*value > max_val) *value = max_val;
    
    return true;
}

// Statistics functions
void reset_validation_stats() {
    memset(&validation_stats, 0, sizeof(validation_stats));
}

void update_validation_stats(ValidationResult result) {
    validation_stats.total_validations++;
    
    if (result == VALIDATION_OK) {
        validation_stats.successful_validations++;
    } else {
        validation_stats.failed_validations++;
        validation_stats.last_error_code = result;
        validation_stats.last_error_timestamp = millis();
    }
}

void print_validation_stats() {
    Serial.println("=== Validation Statistics ===");
    Serial.printf("Total validations: %u\n", validation_stats.total_validations);
    Serial.printf("Successful: %u\n", validation_stats.successful_validations);
    Serial.printf("Failed: %u\n", validation_stats.failed_validations);
    Serial.printf("Sensor errors: %u\n", validation_stats.sensor_errors);
    Serial.printf("Position errors: %u\n", validation_stats.position_errors);
    Serial.printf("Security errors: %u\n", validation_stats.security_errors);
    
    if (validation_stats.failed_validations > 0) {
        Serial.printf("Last error: %s at %u ms\n", 
                      validation_result_to_string(validation_stats.last_error_code),
                      validation_stats.last_error_timestamp);
    }
    
    float success_rate = validation_stats.total_validations > 0 ? 
        (float)validation_stats.successful_validations / validation_stats.total_validations * 100.0f : 0.0f;
    Serial.printf("Success rate: %.1f%%\n", success_rate);
    Serial.println("=============================");
}
