/*
 * OndOcean Standardized Logging System
 * Professional logging with levels, timestamps, and maritime-specific features
 */

#ifndef ONDOCEAN_LOGGER_H
#define ONDOCEAN_LOGGER_H

#include <Arduino.h>
#include <stdarg.h>

// Log levels (ordered by severity)
typedef enum {
    LOG_LEVEL_TRACE = 0,    // Detailed execution flow
    LOG_LEVEL_DEBUG = 1,    // Development debugging
    LOG_LEVEL_INFO = 2,     // General information
    LOG_LEVEL_WARN = 3,     // Warning conditions
    LOG_LEVEL_ERROR = 4,    // Error conditions
    LOG_LEVEL_FATAL = 5,    // Fatal errors (system halt)
    LOG_LEVEL_NONE = 6      // No logging
} LogLevel;

// Log categories for maritime operations
typedef enum {
    LOG_CAT_SYSTEM = 0,     // System initialization, hardware
    LOG_CAT_SENSOR = 1,     // Environmental sensors, IMU
    LOG_CAT_GNSS = 2,       // GPS/GNSS positioning
    LOG_CAT_COMM = 3,       // WiFi, BLE, MQTT communication
    LOG_CAT_MAVLINK = 4,    // MAVLink protocol
    LOG_CAT_ODID = 5,       // OpenDroneID operations
    LOG_CAT_SECURITY = 6,   // Security, tampering, case integrity
    LOG_CAT_POWER = 7,      // Battery, power management
    LOG_CAT_MARITIME = 8,   // Maritime-specific operations
    LOG_CAT_VALIDATION = 9, // Data validation
    LOG_CAT_WEB = 10,       // Web interface
    LOG_CAT_MAX = 11
} LogCategory;

// Logger configuration
struct LoggerConfig {
    LogLevel min_level = LOG_LEVEL_INFO;        // Minimum level to log
    bool enable_timestamps = true;              // Include timestamps
    bool enable_categories = true;              // Include category names
    bool enable_colors = true;                  // ANSI color codes
    bool enable_file_line = false;              // Include file:line info
    bool enable_serial = true;                  // Log to Serial
    bool enable_mqtt_logging = false;           // Log to MQTT (for remote monitoring)
    String mqtt_log_topic = "ondocean/logs";    // MQTT topic for logs
    uint32_t max_log_buffer = 512;              // Maximum log message length
};

// Log statistics
struct LogStats {
    uint32_t total_logs;
    uint32_t logs_by_level[LOG_LEVEL_NONE];
    uint32_t logs_by_category[LOG_CAT_MAX];
    uint32_t dropped_logs;                      // Logs dropped due to buffer overflow
    uint32_t last_log_timestamp;
    LogLevel last_log_level;
    LogCategory last_log_category;
};

// Global logger configuration and stats
extern LoggerConfig logger_config;
extern LogStats log_stats;

// Color codes for different log levels (ANSI)
extern const char* LOG_COLORS[];
extern const char* LOG_RESET;

// Category names
extern const char* LOG_CATEGORY_NAMES[];
extern const char* LOG_LEVEL_NAMES[];

// Core logging functions
void logger_init();
void logger_set_level(LogLevel level);
void logger_enable_mqtt(bool enable, const char* topic = nullptr);
void logger_log(LogLevel level, LogCategory category, const char* format, ...);
void logger_log_raw(const char* message);

// Convenience macros for different log levels
#define LOG_TRACE(cat, fmt, ...)   logger_log(LOG_LEVEL_TRACE, cat, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(cat, fmt, ...)   logger_log(LOG_LEVEL_DEBUG, cat, fmt, ##__VA_ARGS__)
#define LOG_INFO(cat, fmt, ...)    logger_log(LOG_LEVEL_INFO, cat, fmt, ##__VA_ARGS__)
#define LOG_WARN(cat, fmt, ...)    logger_log(LOG_LEVEL_WARN, cat, fmt, ##__VA_ARGS__)
#define LOG_ERROR(cat, fmt, ...)   logger_log(LOG_LEVEL_ERROR, cat, fmt, ##__VA_ARGS__)
#define LOG_FATAL(cat, fmt, ...)   logger_log(LOG_LEVEL_FATAL, cat, fmt, ##__VA_ARGS__)

// Maritime-specific logging macros
#define LOG_MARITIME_INFO(fmt, ...)    LOG_INFO(LOG_CAT_MARITIME, fmt, ##__VA_ARGS__)
#define LOG_MARITIME_WARN(fmt, ...)    LOG_WARN(LOG_CAT_MARITIME, fmt, ##__VA_ARGS__)
#define LOG_MARITIME_ERROR(fmt, ...)   LOG_ERROR(LOG_CAT_MARITIME, fmt, ##__VA_ARGS__)

#define LOG_SENSOR_INFO(fmt, ...)      LOG_INFO(LOG_CAT_SENSOR, fmt, ##__VA_ARGS__)
#define LOG_SENSOR_WARN(fmt, ...)      LOG_WARN(LOG_CAT_SENSOR, fmt, ##__VA_ARGS__)
#define LOG_SENSOR_ERROR(fmt, ...)     LOG_ERROR(LOG_CAT_SENSOR, fmt, ##__VA_ARGS__)

#define LOG_GNSS_INFO(fmt, ...)        LOG_INFO(LOG_CAT_GNSS, fmt, ##__VA_ARGS__)
#define LOG_GNSS_WARN(fmt, ...)        LOG_WARN(LOG_CAT_GNSS, fmt, ##__VA_ARGS__)
#define LOG_GNSS_ERROR(fmt, ...)       LOG_ERROR(LOG_CAT_GNSS, fmt, ##__VA_ARGS__)

#define LOG_COMM_INFO(fmt, ...)        LOG_INFO(LOG_CAT_COMM, fmt, ##__VA_ARGS__)
#define LOG_COMM_WARN(fmt, ...)        LOG_WARN(LOG_CAT_COMM, fmt, ##__VA_ARGS__)
#define LOG_COMM_ERROR(fmt, ...)       LOG_ERROR(LOG_CAT_COMM, fmt, ##__VA_ARGS__)

#define LOG_SECURITY_INFO(fmt, ...)    LOG_INFO(LOG_CAT_SECURITY, fmt, ##__VA_ARGS__)
#define LOG_SECURITY_WARN(fmt, ...)    LOG_WARN(LOG_CAT_SECURITY, fmt, ##__VA_ARGS__)
#define LOG_SECURITY_ERROR(fmt, ...)   LOG_ERROR(LOG_CAT_SECURITY, fmt, ##__VA_ARGS__)

#define LOG_POWER_INFO(fmt, ...)       LOG_INFO(LOG_CAT_POWER, fmt, ##__VA_ARGS__)
#define LOG_POWER_WARN(fmt, ...)       LOG_WARN(LOG_CAT_POWER, fmt, ##__VA_ARGS__)
#define LOG_POWER_ERROR(fmt, ...)      LOG_ERROR(LOG_CAT_POWER, fmt, ##__VA_ARGS__)

#define LOG_VALIDATION_INFO(fmt, ...)  LOG_INFO(LOG_CAT_VALIDATION, fmt, ##__VA_ARGS__)
#define LOG_VALIDATION_WARN(fmt, ...)  LOG_WARN(LOG_CAT_VALIDATION, fmt, ##__VA_ARGS__)
#define LOG_VALIDATION_ERROR(fmt, ...) LOG_ERROR(LOG_CAT_VALIDATION, fmt, ##__VA_ARGS__)

// System state logging
#define LOG_SYSTEM_STARTUP()           LOG_INFO(LOG_CAT_SYSTEM, "OndOcean RemoteID Maritime v1.0 - System startup")
#define LOG_SYSTEM_READY()             LOG_INFO(LOG_CAT_SYSTEM, "System initialization complete - Ready for operation")
#define LOG_SYSTEM_SHUTDOWN()          LOG_INFO(LOG_CAT_SYSTEM, "System shutdown initiated")

// Performance logging
void log_performance_start(const char* operation);
void log_performance_end(const char* operation);

// Statistics and diagnostics
void logger_print_stats();
void logger_reset_stats();
uint32_t logger_get_uptime_ms();
void logger_log_system_info();

// Emergency logging (always logged regardless of level)
#define LOG_EMERGENCY(fmt, ...)        logger_log(LOG_LEVEL_FATAL, LOG_CAT_SYSTEM, "[EMERGENCY] " fmt, ##__VA_ARGS__)

// Conditional logging (only in debug builds)
#ifdef DEBUG
    #define LOG_DEBUG_ONLY(cat, fmt, ...)  LOG_DEBUG(cat, fmt, ##__VA_ARGS__)
#else
    #define LOG_DEBUG_ONLY(cat, fmt, ...)  do {} while(0)
#endif

// Memory-safe logging for large data
void logger_log_hex_dump(LogLevel level, LogCategory category, const char* label, const uint8_t* data, size_t length);
void logger_log_json(LogLevel level, LogCategory category, const char* json_string);

#endif // ONDOCEAN_LOGGER_H
