/*
 * OndOcean Unit Testing Framework
 * Comprehensive testing for critical maritime RemoteID functions
 */

#ifndef UNIT_TESTS_H
#define UNIT_TESTS_H

#include <Arduino.h>
#include "ondocean_logger.h"

// Test result structure
struct TestResult {
    const char* test_name;
    bool passed;
    const char* failure_reason;
    uint32_t execution_time_us;
    uint32_t timestamp_ms;
};

// Test statistics
struct TestStats {
    uint32_t total_tests;
    uint32_t passed_tests;
    uint32_t failed_tests;
    uint32_t skipped_tests;
    uint32_t total_execution_time_us;
    TestResult last_failure;
};

// Global test statistics
extern TestStats test_stats;

// Test framework macros
#define TEST_ASSERT(condition, message) do { \
    if (!(condition)) { \
        test_fail_current(message); \
        return false; \
    } \
} while(0)

#define TEST_ASSERT_EQUAL(expected, actual, message) do { \
    if ((expected) != (actual)) { \
        char fail_msg[128]; \
        snprintf(fail_msg, sizeof(fail_msg), "%s: expected %d, got %d", message, (int)(expected), (int)(actual)); \
        test_fail_current(fail_msg); \
        return false; \
    } \
} while(0)

#define TEST_ASSERT_FLOAT_EQUAL(expected, actual, tolerance, message) do { \
    if (fabs((expected) - (actual)) > (tolerance)) { \
        char fail_msg[128]; \
        snprintf(fail_msg, sizeof(fail_msg), "%s: expected %.3f, got %.3f", message, (float)(expected), (float)(actual)); \
        test_fail_current(fail_msg); \
        return false; \
    } \
} while(0)

#define TEST_ASSERT_NOT_NULL(ptr, message) do { \
    if ((ptr) == nullptr) { \
        test_fail_current(message); \
        return false; \
    } \
} while(0)

#define TEST_ASSERT_NULL(ptr, message) do { \
    if ((ptr) != nullptr) { \
        test_fail_current(message); \
        return false; \
    } \
} while(0)

#define TEST_ASSERT_STRING_EQUAL(expected, actual, message) do { \
    if (strcmp((expected), (actual)) != 0) { \
        char fail_msg[128]; \
        snprintf(fail_msg, sizeof(fail_msg), "%s: expected '%s', got '%s'", message, (expected), (actual)); \
        test_fail_current(fail_msg); \
        return false; \
    } \
} while(0)

// Test framework functions
void test_framework_init();
bool test_run_single(const char* test_name, bool (*test_func)());
void test_run_all();
void test_print_results();
void test_reset_stats();
void test_fail_current(const char* reason);

// Test categories
bool test_run_validation_tests();
bool test_run_sensor_tests();
bool test_run_gnss_tests();
bool test_run_communication_tests();
bool test_run_error_management_tests();
bool test_run_maritime_tests();

// Individual test functions - Data Validation
bool test_validate_maritime_sensor_data_valid();
bool test_validate_maritime_sensor_data_null();
bool test_validate_maritime_sensor_data_out_of_range();
bool test_validate_gnss_position_valid();
bool test_validate_gnss_position_invalid_lat();
bool test_validate_gnss_position_invalid_lon();
bool test_validate_battery_status_normal();
bool test_validate_battery_status_critical();
bool test_sanitize_coordinates();
bool test_sanitize_sensor_reading();

// Individual test functions - Sensor Operations
bool test_sensor_initialization();
bool test_sensor_read_temperature();
bool test_sensor_read_humidity();
bool test_sensor_read_pressure();
bool test_sensor_read_battery_voltage();
bool test_sensor_case_integrity();

// Individual test functions - GNSS Operations
bool test_gnss_initialization();
bool test_gnss_position_parsing();
bool test_gnss_accuracy_validation();
bool test_gnss_maritime_position_check();

// Individual test functions - Communication
bool test_wifi_initialization();
bool test_mqtt_connection();
bool test_mqtt_publish();
bool test_ble_initialization();
bool test_json_serialization();

// Individual test functions - Error Management
bool test_error_reporting();
bool test_error_recovery();
bool test_error_severity_mapping();
bool test_error_statistics();

// Individual test functions - Maritime Operations
bool test_maritime_environment_safety();
bool test_maritime_altitude_limits();
bool test_maritime_seal_integrity();
bool test_maritime_weather_conditions();

// Mock functions for testing
void mock_sensor_data_valid(MaritimeSensorData* data);
void mock_sensor_data_invalid(MaritimeSensorData* data);
void mock_gnss_data_valid(double* lat, double* lon, float* alt);
void mock_gnss_data_invalid(double* lat, double* lon, float* alt);

// Performance testing
bool test_performance_sensor_read();
bool test_performance_validation();
bool test_performance_json_serialization();

// Stress testing
bool test_stress_memory_allocation();
bool test_stress_continuous_operation();
bool test_stress_error_recovery();

// Integration testing
bool test_integration_sensor_to_mqtt();
bool test_integration_gnss_to_validation();
bool test_integration_error_to_recovery();

#endif // UNIT_TESTS_H
