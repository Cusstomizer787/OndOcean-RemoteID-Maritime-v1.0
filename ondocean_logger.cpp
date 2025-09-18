/*
 * OndOcean Standardized Logging System Implementation
 * Professional logging with levels, timestamps, and maritime-specific features
 */

#include "ondocean_logger.h"
#include <WiFi.h>
#include <PubSubClient.h>

// Global logger configuration and statistics
LoggerConfig logger_config;
LogStats log_stats = {0};

// ANSI color codes for different log levels
const char* LOG_COLORS[] = {
    "\033[37m",     // TRACE - White
    "\033[36m",     // DEBUG - Cyan  
    "\033[32m",     // INFO - Green
    "\033[33m",     // WARN - Yellow
    "\033[31m",     // ERROR - Red
    "\033[35m",     // FATAL - Magenta
    "\033[0m"       // NONE - Reset
};

const char* LOG_RESET = "\033[0m";

// Log level names
const char* LOG_LEVEL_NAMES[] = {
    "TRACE", "DEBUG", "INFO ", "WARN ", "ERROR", "FATAL", "NONE "
};

// Category names for maritime operations
const char* LOG_CATEGORY_NAMES[] = {
    "SYSTEM", "SENSOR", "GNSS  ", "COMM  ", "MAVLNK", 
    "ODID  ", "SECUR ", "POWER ", "MARIT ", "VALID ", "WEB   "
};

// Performance tracking
static uint32_t performance_start_time = 0;
static uint32_t system_start_time = 0;

// External MQTT client (defined in main code) - temporarily disabled
// extern PubSubClient mqttClient;

void logger_init() {
    system_start_time = millis();
    
    // Initialize default configuration
    logger_config.min_level = LOG_LEVEL_INFO;
    logger_config.enable_timestamps = true;
    logger_config.enable_categories = true;
    logger_config.enable_colors = true;
    logger_config.enable_serial = true;
    logger_config.enable_mqtt_logging = false;
    
    // Reset statistics
    logger_reset_stats();
    
    // Log system startup
    LOG_SYSTEM_STARTUP();
    logger_log_system_info();
}

void logger_set_level(LogLevel level) {
    logger_config.min_level = level;
    LOG_INFO(LOG_CAT_SYSTEM, "Log level set to %s", LOG_LEVEL_NAMES[level]);
}

void logger_enable_mqtt(bool enable, const char* topic) {
    logger_config.enable_mqtt_logging = enable;
    if (topic) {
        logger_config.mqtt_log_topic = String(topic);
    }
    LOG_INFO(LOG_CAT_SYSTEM, "MQTT logging %s (topic: %s)", 
             enable ? "enabled" : "disabled", 
             logger_config.mqtt_log_topic.c_str());
}

void logger_log(LogLevel level, LogCategory category, const char* format, ...) {
    // Check if this log level should be processed
    if (level < logger_config.min_level) {
        return;
    }
    
    // Update statistics
    log_stats.total_logs++;
    if (level < LOG_LEVEL_NONE) {
        log_stats.logs_by_level[level]++;
    }
    if (category < LOG_CAT_MAX) {
        log_stats.logs_by_category[category]++;
    }
    log_stats.last_log_timestamp = millis();
    log_stats.last_log_level = level;
    log_stats.last_log_category = category;
    
    // Create log message buffer
    char log_buffer[logger_config.max_log_buffer];
    char* buffer_ptr = log_buffer;
    size_t remaining_space = sizeof(log_buffer);
    
    // Add timestamp if enabled
    if (logger_config.enable_timestamps) {
        uint32_t timestamp = millis();
        int written = snprintf(buffer_ptr, remaining_space, "[%08u] ", timestamp);
        if (written > 0 && written < remaining_space) {
            buffer_ptr += written;
            remaining_space -= written;
        }
    }
    
    // Add color if enabled and logging to serial
    if (logger_config.enable_colors && logger_config.enable_serial) {
        int written = snprintf(buffer_ptr, remaining_space, "%s", LOG_COLORS[level]);
        if (written > 0 && written < remaining_space) {
            buffer_ptr += written;
            remaining_space -= written;
        }
    }
    
    // Add log level
    int written = snprintf(buffer_ptr, remaining_space, "[%s]", LOG_LEVEL_NAMES[level]);
    if (written > 0 && written < remaining_space) {
        buffer_ptr += written;
        remaining_space -= written;
    }
    
    // Add category if enabled
    if (logger_config.enable_categories) {
        written = snprintf(buffer_ptr, remaining_space, "[%s]", LOG_CATEGORY_NAMES[category]);
        if (written > 0 && written < remaining_space) {
            buffer_ptr += written;
            remaining_space -= written;
        }
    }
    
    // Add separator
    written = snprintf(buffer_ptr, remaining_space, " ");
    if (written > 0 && written < remaining_space) {
        buffer_ptr += written;
        remaining_space -= written;
    }
    
    // Add formatted message
    va_list args;
    va_start(args, format);
    written = vsnprintf(buffer_ptr, remaining_space, format, args);
    va_end(args);
    
    if (written > 0 && written < remaining_space) {
        buffer_ptr += written;
        remaining_space -= written;
    } else {
        // Buffer overflow - mark as dropped
        log_stats.dropped_logs++;
        snprintf(buffer_ptr - 20, 20, "...[TRUNCATED]");
    }
    
    // Add color reset if enabled
    if (logger_config.enable_colors && logger_config.enable_serial) {
        written = snprintf(buffer_ptr, remaining_space, "%s", LOG_RESET);
        if (written > 0 && written < remaining_space) {
            buffer_ptr += written;
            remaining_space -= written;
        }
    }
    
    // Output to Serial if enabled
    if (logger_config.enable_serial) {
        Serial.println(log_buffer);
    }
    
    // Output to MQTT if enabled and connected
    if (logger_config.enable_mqtt_logging) {  // MQTT logging temporarily disabled - mqttClient not defined
        // Create simplified message for MQTT (no colors, shorter format)
        char mqtt_buffer[256];
        snprintf(mqtt_buffer, sizeof(mqtt_buffer), 
                 "{\"timestamp\":%u,\"level\":\"%s\",\"category\":\"%s\",\"message\":\"%s\"}", 
                 millis(), LOG_LEVEL_NAMES[level], LOG_CATEGORY_NAMES[category], format);
        
        // mqttClient.publish(mqtt_topic, buffer);
    }
    
    // For FATAL errors, halt the system after logging
    if (level == LOG_LEVEL_FATAL) {
        Serial.println("[FATAL] System halted due to fatal error");
        while (true) {
            delay(1000);  // Infinite loop - system halt
        }
    }
}

void logger_log_raw(const char* message) {
    if (logger_config.enable_serial) {
        Serial.println(message);
    }
    log_stats.total_logs++;
}

void log_performance_start(const char* operation) {
    performance_start_time = millis();
    LOG_DEBUG(LOG_CAT_SYSTEM, "Performance: %s started", operation);
}

void log_performance_end(const char* operation) {
    uint32_t duration = millis() - performance_start_time;
    LOG_DEBUG(LOG_CAT_SYSTEM, "Performance: %s completed in %u ms", operation, duration);
}

void logger_print_stats() {
    LOG_INFO(LOG_CAT_SYSTEM, "=== Logging Statistics ===");
    LOG_INFO(LOG_CAT_SYSTEM, "Total logs: %u", log_stats.total_logs);
    LOG_INFO(LOG_CAT_SYSTEM, "Dropped logs: %u", log_stats.dropped_logs);
    
    // Log counts by level
    LOG_INFO(LOG_CAT_SYSTEM, "By level:");
    for (int i = 0; i < LOG_LEVEL_NONE; i++) {
        if (log_stats.logs_by_level[i] > 0) {
            LOG_INFO(LOG_CAT_SYSTEM, "  %s: %u", LOG_LEVEL_NAMES[i], log_stats.logs_by_level[i]);
        }
    }
    
    // Log counts by category
    LOG_INFO(LOG_CAT_SYSTEM, "By category:");
    for (int i = 0; i < LOG_CAT_MAX; i++) {
        if (log_stats.logs_by_category[i] > 0) {
            LOG_INFO(LOG_CAT_SYSTEM, "  %s: %u", LOG_CATEGORY_NAMES[i], log_stats.logs_by_category[i]);
        }
    }
    
    if (log_stats.total_logs > 0) {
        LOG_INFO(LOG_CAT_SYSTEM, "Last log: %s/%s at %u ms", 
                 LOG_LEVEL_NAMES[log_stats.last_log_level],
                 LOG_CATEGORY_NAMES[log_stats.last_log_category],
                 log_stats.last_log_timestamp);
    }
    
    LOG_INFO(LOG_CAT_SYSTEM, "System uptime: %u ms", logger_get_uptime_ms());
    LOG_INFO(LOG_CAT_SYSTEM, "========================");
}

void logger_reset_stats() {
    memset(&log_stats, 0, sizeof(log_stats));
    LOG_INFO(LOG_CAT_SYSTEM, "Logging statistics reset");
}

uint32_t logger_get_uptime_ms() {
    return millis() - system_start_time;
}

void logger_log_system_info() {
    LOG_INFO(LOG_CAT_SYSTEM, "ESP32-S3 Chip ID: %llX", ESP.getEfuseMac());
    LOG_INFO(LOG_CAT_SYSTEM, "Flash size: %u bytes", ESP.getFlashChipSize());
    LOG_INFO(LOG_CAT_SYSTEM, "Free heap: %u bytes", ESP.getFreeHeap());
    LOG_INFO(LOG_CAT_SYSTEM, "CPU frequency: %u MHz", ESP.getCpuFreqMHz());
    
    if (WiFi.status() == WL_CONNECTED) {
        LOG_INFO(LOG_CAT_SYSTEM, "WiFi connected: %s (RSSI: %d dBm)", 
                 WiFi.SSID().c_str(), WiFi.RSSI());
        LOG_INFO(LOG_CAT_SYSTEM, "IP address: %s", WiFi.localIP().toString().c_str());
    } else {
        LOG_INFO(LOG_CAT_SYSTEM, "WiFi not connected");
    }
}

void logger_log_hex_dump(LogLevel level, LogCategory category, const char* label, const uint8_t* data, size_t length) {
    if (level < logger_config.min_level || !data) {
        return;
    }
    
    LOG_DEBUG(category, "Hex dump: %s (%u bytes)", label, length);
    
    char hex_line[80];
    for (size_t i = 0; i < length; i += 16) {
        char* ptr = hex_line;
        ptr += sprintf(ptr, "%04X: ", (unsigned int)i);
        
        // Hex bytes
        for (size_t j = 0; j < 16 && (i + j) < length; j++) {
            ptr += sprintf(ptr, "%02X ", data[i + j]);
        }
        
        // Padding for incomplete lines
        for (size_t j = length - i; j < 16; j++) {
            ptr += sprintf(ptr, "   ");
        }
        
        ptr += sprintf(ptr, " ");
        
        // ASCII representation
        for (size_t j = 0; j < 16 && (i + j) < length; j++) {
            uint8_t c = data[i + j];
            *ptr++ = (c >= 32 && c <= 126) ? c : '.';
        }
        *ptr = '\0';
        
        logger_log(level, category, "%s", hex_line);
    }
}

void logger_log_json(LogLevel level, LogCategory category, const char* json_string) {
    if (level < logger_config.min_level || !json_string) {
        return;
    }
    
    // Pretty print JSON (simplified)
    LOG_DEBUG(category, "JSON data: %s", json_string);
}
