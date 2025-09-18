/*
 * OndOcean MQTT Integration
 * Maritime RemoteID telemetry and command interface
 */

#ifndef ONDOCEAN_MQTT_H
#define ONDOCEAN_MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "maritime_sensors.h"

// MQTT Topics
#define MQTT_TOPIC_BASE         "ondocean/remoteid"
#define MQTT_TOPIC_STATUS       MQTT_TOPIC_BASE "/status"
#define MQTT_TOPIC_POSITION     MQTT_TOPIC_BASE "/position"
#define MQTT_TOPIC_TELEMETRY    MQTT_TOPIC_BASE "/telemetry"
#define MQTT_TOPIC_EMERGENCY    MQTT_TOPIC_BASE "/emergency"
#define MQTT_TOPIC_CONFIG       MQTT_TOPIC_BASE "/config"
#define MQTT_TOPIC_COMMAND      MQTT_TOPIC_BASE "/command"

// MQTT Configuration
struct MQTTConfig {
    String broker_host;
    uint16_t broker_port;
    String device_id;
    String username;
    String password;
    uint16_t keepalive_sec;
    uint8_t qos_level;
    bool retain_messages;
};

// MQTT Message Types
enum MQTTMessageType {
    MSG_STATUS = 0,
    MSG_POSITION = 1,
    MSG_TELEMETRY = 2,
    MSG_EMERGENCY = 3,
    MSG_CONFIG = 4
};

// Position data structure for MQTT
struct PositionData {
    double latitude;
    double longitude;
    float altitude_msl;
    float altitude_agl;
    float ground_speed;
    float heading;
    float accuracy_horizontal;
    float accuracy_vertical;
    uint32_t timestamp_utc;
    uint8_t satellites_used;
    uint8_t fix_quality;
};

// Device status structure
struct DeviceStatus {
    String firmware_version;
    String hardware_revision;
    uint32_t uptime_seconds;
    float cpu_temperature;
    uint8_t wifi_rssi;
    bool gnss_fix_valid;
    bool remoteid_transmitting;
    bool emergency_mode;
    uint16_t error_flags;
};

// Function declarations
bool mqtt_init(const MQTTConfig& config);
void mqtt_loop();
bool mqtt_is_connected();
void mqtt_reconnect();

// Publishing functions
bool mqtt_publish_status(const DeviceStatus& status);
bool mqtt_publish_position(const PositionData& position);
bool mqtt_publish_telemetry(const MaritimeSensorData& sensors);
bool mqtt_publish_emergency(const String& emergency_type, const String& message);

// Subscription and command handling
void mqtt_subscribe_commands();
void mqtt_handle_command(const String& command, const JsonDocument& payload);

// Utility functions
String mqtt_create_device_id();
void mqtt_set_last_will();
bool mqtt_validate_connection();

// Emergency beacon functions
void mqtt_emergency_beacon_start(const String& reason);
void mqtt_emergency_beacon_stop();
bool mqtt_is_emergency_active();

#endif // ONDOCEAN_MQTT_H
