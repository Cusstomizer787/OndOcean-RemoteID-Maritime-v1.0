/*
 * OndOcean Error Management System
 * Comprehensive error handling for maritime RemoteID operations
 */

#ifndef ERROR_MANAGEMENT_H
#define ERROR_MANAGEMENT_H

#include <Arduino.h>
#include "ondocean_logger.h"

// Error codes for different subsystems
typedef enum {
    // Success
    ERROR_NONE = 0,
    
    // System errors (1-99)
    ERROR_SYSTEM_INIT_FAILED = 1,
    ERROR_SYSTEM_LOW_MEMORY = 2,
    ERROR_SYSTEM_WATCHDOG_TIMEOUT = 3,
    ERROR_SYSTEM_BROWNOUT = 4,
    ERROR_SYSTEM_OVERHEATING = 5,
    
    // Hardware errors (100-199)
    ERROR_HW_I2C_FAILURE = 100,
    ERROR_HW_SPI_FAILURE = 101,
    ERROR_HW_UART_FAILURE = 102,
    ERROR_HW_GPIO_FAILURE = 103,
    ERROR_HW_POWER_FAILURE = 104,
    ERROR_HW_CLOCK_FAILURE = 105,
    
    // Sensor errors (200-299)
    ERROR_SENSOR_BME280_NOT_FOUND = 200,
    ERROR_SENSOR_BME280_READ_FAILED = 201,
    ERROR_SENSOR_IMU_NOT_FOUND = 202,
    ERROR_SENSOR_IMU_CALIBRATION_FAILED = 203,
    ERROR_SENSOR_MAGNETOMETER_NOT_FOUND = 204,
    ERROR_SENSOR_BATTERY_READ_FAILED = 205,
    ERROR_SENSOR_CASE_TAMPER_DETECTED = 206,
    
    // GNSS errors (300-399)
    ERROR_GNSS_NOT_FOUND = 300,
    ERROR_GNSS_NO_FIX = 301,
    ERROR_GNSS_POOR_ACCURACY = 302,
    ERROR_GNSS_ANTENNA_FAULT = 303,
    ERROR_GNSS_JAMMING_DETECTED = 304,
    ERROR_GNSS_SPOOFING_DETECTED = 305,
    
    // Communication errors (400-499)
    ERROR_COMM_WIFI_INIT_FAILED = 400,
    ERROR_COMM_WIFI_CONNECTION_LOST = 401,
    ERROR_COMM_MQTT_INIT_FAILED = 402,
    ERROR_COMM_MQTT_CONNECTION_LOST = 403,
    ERROR_COMM_BLE_INIT_FAILED = 404,
    ERROR_COMM_BLE_ADVERTISING_FAILED = 405,
    ERROR_COMM_NETWORK_TIMEOUT = 406,
    
    // Protocol errors (500-599)
    ERROR_PROTOCOL_MAVLINK_PARSE_FAILED = 500,
    ERROR_PROTOCOL_MAVLINK_CRC_FAILED = 501,
    ERROR_PROTOCOL_ODID_INVALID_DATA = 502,
    ERROR_PROTOCOL_ODID_ENCODING_FAILED = 503,
    ERROR_PROTOCOL_JSON_PARSE_FAILED = 504,
    
    // Security errors (600-699)
    ERROR_SECURITY_CASE_OPENED = 600,
    ERROR_SECURITY_TAMPER_DETECTED = 601,
    ERROR_SECURITY_CRYPTO_FAILED = 602,
    ERROR_SECURITY_SIGNATURE_INVALID = 603,
    ERROR_SECURITY_UNAUTHORIZED_ACCESS = 604,
    
    // Maritime errors (700-799)
    ERROR_MARITIME_ENVIRONMENT_UNSAFE = 700,
    ERROR_MARITIME_NOT_OVER_WATER = 701,
    ERROR_MARITIME_ALTITUDE_EXCEEDED = 702,
    ERROR_MARITIME_WEATHER_SEVERE = 703,
    ERROR_MARITIME_SEAL_COMPROMISED = 704,
    
    // Validation errors (800-899)
    ERROR_VALIDATION_NULL_POINTER = 800,
    ERROR_VALIDATION_OUT_OF_RANGE = 801,
    ERROR_VALIDATION_INVALID_FORMAT = 802,
    ERROR_VALIDATION_CHECKSUM_FAILED = 803,
    ERROR_VALIDATION_TIMEOUT = 804,
    
    // Storage errors (900-999)
    ERROR_STORAGE_NVS_INIT_FAILED = 900,
    ERROR_STORAGE_NVS_READ_FAILED = 901,
    ERROR_STORAGE_NVS_WRITE_FAILED = 902,
    ERROR_STORAGE_FLASH_CORRUPTED = 903,
    ERROR_STORAGE_EEPROM_FAILED = 904,
    
    ERROR_MAX = 1000
} ErrorCode;

// Error severity levels
typedef enum {
    ERROR_SEVERITY_INFO = 0,        // Informational, no action needed
    ERROR_SEVERITY_WARNING = 1,     // Warning, system can continue
    ERROR_SEVERITY_ERROR = 2,       // Error, functionality impacted
    ERROR_SEVERITY_CRITICAL = 3,    // Critical, system stability at risk
    ERROR_SEVERITY_FATAL = 4        // Fatal, system must halt
} ErrorSeverity;

// Error context information
struct ErrorContext {
    ErrorCode code;
    ErrorSeverity severity;
    uint32_t timestamp_ms;
    const char* function_name;
    const char* file_name;
    int line_number;
    char description[128];
    uint32_t additional_data;
    bool recovery_attempted;
    bool recovery_successful;
};

// Error statistics
struct ErrorStats {
    uint32_t total_errors;
    uint32_t errors_by_severity[5];
    uint32_t errors_by_category[10];  // System, HW, Sensor, etc.
    uint32_t recovery_attempts;
    uint32_t successful_recoveries;
    ErrorContext last_error;
    ErrorContext last_critical_error;
};

// Global error statistics
extern ErrorStats error_stats;

// Error management functions
void error_manager_init();
void error_report(ErrorCode code, const char* function, const char* file, int line, const char* description = nullptr, uint32_t data = 0);
void error_report_with_recovery(ErrorCode code, const char* function, const char* file, int line, bool (*recovery_func)(), const char* description = nullptr);

// Error recovery functions
bool error_attempt_recovery(ErrorCode code);
bool error_is_recoverable(ErrorCode code);
ErrorSeverity error_get_severity(ErrorCode code);
const char* error_get_description(ErrorCode code);
const char* error_get_category_name(ErrorCode code);

// Error statistics and reporting
void error_print_stats();
void error_reset_stats();
void error_log_system_state();

// Macros for easy error reporting
#define REPORT_ERROR(code, desc) \
    error_report(code, __FUNCTION__, __FILE__, __LINE__, desc)

#define REPORT_ERROR_WITH_DATA(code, desc, data) \
    error_report(code, __FUNCTION__, __FILE__, __LINE__, desc, data)

#define REPORT_ERROR_WITH_RECOVERY(code, recovery_func, desc) \
    error_report_with_recovery(code, __FUNCTION__, __FILE__, __LINE__, recovery_func, desc)

// Critical error handling
#define CRITICAL_ERROR(code, desc) do { \
    REPORT_ERROR(code, desc); \
    LOG_FATAL(LOG_CAT_SYSTEM, "CRITICAL ERROR: %s - System halting", desc); \
} while(0)

// Error checking macros
#define CHECK_ERROR(condition, code, desc) do { \
    if (!(condition)) { \
        REPORT_ERROR(code, desc); \
        return false; \
    } \
} while(0)

#define CHECK_ERROR_RETURN(condition, code, desc, return_value) do { \
    if (!(condition)) { \
        REPORT_ERROR(code, desc); \
        return return_value; \
    } \
} while(0)

// Recovery strategy definitions
typedef enum {
    RECOVERY_NONE = 0,
    RECOVERY_RETRY = 1,
    RECOVERY_RESET_SUBSYSTEM = 2,
    RECOVERY_SAFE_MODE = 3,
    RECOVERY_RESTART_SYSTEM = 4,
    RECOVERY_HALT_SYSTEM = 5
} RecoveryStrategy;

// Recovery function prototypes
bool recovery_retry_operation();
bool recovery_reset_i2c_bus();
bool recovery_reset_wifi();
bool recovery_reset_mqtt();
bool recovery_reset_gnss();
bool recovery_enter_safe_mode();
bool recovery_restart_system();

// Watchdog and health monitoring
void error_watchdog_init();
void error_watchdog_feed();
void error_health_check();
bool error_is_system_healthy();

// Emergency procedures
void emergency_shutdown();
void emergency_safe_mode();
void emergency_data_backup();

#endif // ERROR_MANAGEMENT_H
