/*
 * OndOcean MQTT Integration Implementation
 * Maritime RemoteID telemetry and command interface
 */

#include "ondocean_mqtt.h"
#include "board_config_maritime.h"
#include <WiFi.h>

// Global MQTT client and configuration
static WiFiClient wifi_client;
static PubSubClient mqtt_client(wifi_client);
static MQTTConfig mqtt_config;
static bool mqtt_initialized = false;
static bool emergency_beacon_active = false;

// Internal function declarations
static void mqtt_reconnect_internal();
static String create_json_message(MQTTMessageType type, const JsonDocument& data);

bool mqtt_init(const MQTTConfig& config) {
    mqtt_config = config;
    
    // Set MQTT server
    mqtt_client.setServer(config.broker_host.c_str(), config.broker_port);
    
    // Set buffer size for larger messages
    mqtt_client.setBufferSize(1024);
    
    // Set keepalive
    mqtt_client.setKeepAlive(config.keepalive_sec);
    
    // Set callback for incoming messages
    mqtt_client.setCallback([](char* topic, byte* payload, unsigned int length) {
        String message;
        for (unsigned int i = 0; i < length; i++) {
            message += (char)payload[i];
        }
        
        Serial.printf("MQTT received [%s]: %s\n", topic, message.c_str());
        
        // Parse and handle command
        if (String(topic).startsWith(MQTT_TOPIC_COMMAND)) {
            DynamicJsonDocument doc(512);
            if (deserializeJson(doc, message) == DeserializationError::Ok) {
                mqtt_handle_command(String(topic), doc);
            }
        }
    });
    
    mqtt_initialized = true;
    Serial.println("MQTT initialized");
    
    return true;
}

void mqtt_loop() {
    if (!mqtt_initialized) {
        return;
    }
    
    if (!mqtt_client.connected()) {
        mqtt_reconnect_internal();
    }
    
    mqtt_client.loop();
}

bool mqtt_is_connected() {
    return mqtt_initialized && mqtt_client.connected();
}

void mqtt_reconnect() {
    mqtt_reconnect_internal();
}

static void mqtt_reconnect_internal() {
    if (!mqtt_initialized) {
        return;
    }
    
    // Attempt to connect
    Serial.print("Attempting MQTT connection...");
    
    String client_id = mqtt_config.device_id;
    if (client_id.isEmpty()) {
        client_id = mqtt_create_device_id();
    }
    
    // Set last will and testament
    String lwt_topic = String(MQTT_TOPIC_STATUS) + "/" + client_id;
    String lwt_message = "{\"status\":\"offline\",\"timestamp\":" + String(millis()) + "}";
    
    bool connected = false;
    if (!mqtt_config.username.isEmpty()) {
        connected = mqtt_client.connect(
            client_id.c_str(),
            mqtt_config.username.c_str(),
            mqtt_config.password.c_str(),
            lwt_topic.c_str(),
            mqtt_config.qos_level,
            mqtt_config.retain_messages,
            lwt_message.c_str()
        );
    } else {
        connected = mqtt_client.connect(
            client_id.c_str(),
            lwt_topic.c_str(),
            mqtt_config.qos_level,
            mqtt_config.retain_messages,
            lwt_message.c_str()
        );
    }
    
    if (connected) {
        Serial.println(" connected");
        
        // Subscribe to command topics
        mqtt_subscribe_commands();
        
        // Publish online status
        DynamicJsonDocument status_doc(256);
        status_doc["status"] = "online";
        status_doc["timestamp"] = millis();
        status_doc["device_id"] = client_id;
        
        String status_msg;
        serializeJson(status_doc, status_msg);
        mqtt_client.publish(lwt_topic.c_str(), status_msg.c_str(), mqtt_config.retain_messages);
        
    } else {
        Serial.printf(" failed, rc=%d\n", mqtt_client.state());
    }
}

bool mqtt_publish_status(const DeviceStatus& status) {
    if (!mqtt_is_connected()) {
        return false;
    }
    
    DynamicJsonDocument doc(512);
    doc["header"]["timestamp"] = millis();
    doc["header"]["device_id"] = mqtt_config.device_id;
    doc["header"]["message_type"] = "status";
    
    doc["status"]["firmware_version"] = status.firmware_version;
    doc["status"]["hardware_revision"] = status.hardware_revision;
    doc["status"]["uptime_seconds"] = status.uptime_seconds;
    doc["status"]["cpu_temperature"] = status.cpu_temperature;
    doc["status"]["wifi_rssi"] = status.wifi_rssi;
    doc["status"]["gnss_fix_valid"] = status.gnss_fix_valid;
    doc["status"]["remoteid_transmitting"] = status.remoteid_transmitting;
    doc["status"]["emergency_mode"] = status.emergency_mode;
    doc["status"]["error_flags"] = status.error_flags;
    
    String message;
    serializeJson(doc, message);
    
    String topic = String(MQTT_TOPIC_STATUS) + "/" + mqtt_config.device_id;
    return mqtt_client.publish(topic.c_str(), message.c_str(), mqtt_config.retain_messages);
}

bool mqtt_publish_position(const PositionData& position) {
    if (!mqtt_is_connected()) {
        return false;
    }
    
    DynamicJsonDocument doc(512);
    doc["header"]["timestamp"] = millis();
    doc["header"]["device_id"] = mqtt_config.device_id;
    doc["header"]["message_type"] = "position";
    
    doc["position"]["latitude"] = position.latitude;
    doc["position"]["longitude"] = position.longitude;
    doc["position"]["altitude_msl"] = position.altitude_msl;
    doc["position"]["altitude_agl"] = position.altitude_agl;
    doc["position"]["ground_speed"] = position.ground_speed;
    doc["position"]["heading"] = position.heading;
    doc["position"]["accuracy_horizontal"] = position.accuracy_horizontal;
    doc["position"]["accuracy_vertical"] = position.accuracy_vertical;
    doc["position"]["timestamp_utc"] = position.timestamp_utc;
    doc["position"]["satellites_used"] = position.satellites_used;
    doc["position"]["fix_quality"] = position.fix_quality;
    
    String message;
    serializeJson(doc, message);
    
    String topic = String(MQTT_TOPIC_POSITION) + "/" + mqtt_config.device_id;
    return mqtt_client.publish(topic.c_str(), message.c_str(), mqtt_config.retain_messages);
}

bool mqtt_publish_telemetry(const MaritimeSensorData& sensors) {
    if (!mqtt_is_connected()) {
        return false;
    }
    
    DynamicJsonDocument doc(512);
    doc["header"]["timestamp"] = millis();
    doc["header"]["device_id"] = mqtt_config.device_id;
    doc["header"]["message_type"] = "telemetry";
    
    doc["sensors"]["temperature_c"] = sensors.temperature_c;
    doc["sensors"]["humidity_pct"] = sensors.humidity_pct;
    doc["sensors"]["pressure_hpa"] = sensors.pressure_hpa;
    doc["sensors"]["battery_voltage"] = sensors.battery_voltage;
    doc["sensors"]["case_tamper_detected"] = sensors.case_tamper_detected;
    doc["sensors"]["timestamp_ms"] = sensors.timestamp_ms;
    
    doc["imu"]["accel_x"] = sensors.accel_x;
    doc["imu"]["accel_y"] = sensors.accel_y;
    doc["imu"]["accel_z"] = sensors.accel_z;
    doc["imu"]["gyro_x"] = sensors.gyro_x;
    doc["imu"]["gyro_y"] = sensors.gyro_y;
    doc["imu"]["gyro_z"] = sensors.gyro_z;
    doc["imu"]["mag_x"] = sensors.mag_x;
    doc["imu"]["mag_y"] = sensors.mag_y;
    doc["imu"]["mag_z"] = sensors.mag_z;
    
    String message;
    serializeJson(doc, message);
    
    String topic = String(MQTT_TOPIC_TELEMETRY) + "/" + mqtt_config.device_id;
    return mqtt_client.publish(topic.c_str(), message.c_str(), mqtt_config.retain_messages);
}

bool mqtt_publish_emergency(const String& emergency_type, const String& message) {
    if (!mqtt_is_connected()) {
        return false;
    }
    
    DynamicJsonDocument doc(256);
    doc["header"]["timestamp"] = millis();
    doc["header"]["device_id"] = mqtt_config.device_id;
    doc["header"]["message_type"] = "emergency";
    
    doc["emergency"]["type"] = emergency_type;
    doc["emergency"]["message"] = message;
    doc["emergency"]["active"] = emergency_beacon_active;
    
    String json_message;
    serializeJson(doc, json_message);
    
    String topic = String(MQTT_TOPIC_EMERGENCY) + "/" + mqtt_config.device_id;
    return mqtt_client.publish(topic.c_str(), json_message.c_str(), true); // Always retain emergency messages
}

void mqtt_subscribe_commands() {
    if (!mqtt_is_connected()) {
        return;
    }
    
    String command_topic = String(MQTT_TOPIC_COMMAND) + "/" + mqtt_config.device_id + "/+";
    mqtt_client.subscribe(command_topic.c_str(), mqtt_config.qos_level);
    
    Serial.printf("Subscribed to commands: %s\n", command_topic.c_str());
}

void mqtt_handle_command(const String& topic, const JsonDocument& payload) {
    Serial.printf("Handling command from topic: %s\n", topic.c_str());
    
    // Extract command type from topic
    String command_type = topic.substring(topic.lastIndexOf('/') + 1);
    
    if (command_type == "reboot") {
        Serial.println("Reboot command received");
        delay(1000);
        ESP.restart();
    }
    else if (command_type == "emergency_start") {
        String reason = payload["reason"].as<String>();
        mqtt_emergency_beacon_start(reason);
    }
    else if (command_type == "emergency_stop") {
        mqtt_emergency_beacon_stop();
    }
    else if (command_type == "diagnostics") {
        // Trigger diagnostics
        Serial.println("Diagnostics command received");
        // TODO: Implement diagnostics routine
    }
    else {
        Serial.printf("Unknown command: %s\n", command_type.c_str());
    }
}

String mqtt_create_device_id() {
    String mac = WiFi.macAddress();
    mac.replace(":", "");
    return "ONRID-" + mac;
}

void mqtt_set_last_will() {
    // Last will is set during connection
}

bool mqtt_validate_connection() {
    return mqtt_is_connected();
}

void mqtt_emergency_beacon_start(const String& reason) {
    emergency_beacon_active = true;
    Serial.printf("Emergency beacon started: %s\n", reason.c_str());
    mqtt_publish_emergency("beacon_start", reason);
}

void mqtt_emergency_beacon_stop() {
    emergency_beacon_active = false;
    Serial.println("Emergency beacon stopped");
    mqtt_publish_emergency("beacon_stop", "Manual stop");
}

bool mqtt_is_emergency_active() {
    return emergency_beacon_active;
}
