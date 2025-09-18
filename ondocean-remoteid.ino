/*
 * OndOcean RemoteID Maritime Module
 * Based on ArduRemoteID with maritime adaptations
 * ESP32-S3 firmware for maritime drone identification
 */

#include "options.h"
#include <Arduino.h>
#include "version.h"
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <opendroneid.h>
#include "mavlink.h"
#if AP_DRONECAN_ENABLED
#include "DroneCAN.h"
#endif
#include "WiFi_TX.h"
#include "BLE_TX.h"
#include <esp_wifi.h>
#include <WiFi.h>
#include "parameters.h"
#include "webinterface.h"
#include "check_firmware.h"
#include <esp_ota_ops.h>
#include "efuse.h"
#include "led.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>

// OndOcean Maritime specific includes
#include "board_config_maritime.h"
#include "maritime_sensors.h"
#include "ondocean_mqtt.h"
#include "data_validation.h"
#include "ondocean_logger.h"
// #include "error_management.h"  // Temporarily disabled for compilation
// #include "unit_tests.h"        // Temporarily disabled for compilation

#if AP_DRONECAN_ENABLED
static DroneCAN dronecan;
#endif

#if AP_MAVLINK_ENABLED
static MAVLinkSerial mavlink1{Serial1, MAVLINK_COMM_0};
static MAVLinkSerial mavlink2{Serial,  MAVLINK_COMM_1};
#endif

static WiFi_TX wifi;
static BLE_TX ble;
static Parameters parameters;

// OndOcean Maritime components
static HardwareSerial gnssSerial(1);  // UART1 for GNSS
static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);
static WebInterface webif;

#define DEBUG_BAUDRATE 57600

// OpenDroneID output data structure
ODID_UAS_Data UAS_data;

// MAVLink system configuration (extern declaration - defined in mavlink.cpp)
extern mavlink_system_t mavlink_system;
String status_reason;
static uint32_t last_location_ms;
static uint32_t last_mqtt_publish_ms;

// Maritime configuration
struct MaritimeConfig {
    bool maritime_mode_enabled = true;
    bool gnss_required = true;
    bool mqtt_enabled = true;
    String device_id;
    String mqtt_broker = "anémone.local";
    int mqtt_port = 1883;
    String mqtt_topic_prefix = "ondocean/remoteid";
    
    // Maritime position
    double latitude = 0.0;
    double longitude = 0.0;
    float altitude = 0.0;
    float accuracy = 0.0;
    bool position_valid = false;
    
    // Environmental sensors
    float temperature = 0.0;
    float humidity = 0.0;
    float pressure = 0.0;
    
    // Power management
    float battery_voltage = 0.0;
    bool low_power_mode = false;
    
    // Case status
    bool case_closed = true;
    bool waterproof_sealed = true;
} maritime_config;

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

void setup() {
    // Disable brownout detector
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    
    Serial.begin(DEBUG_BAUDRATE);
    delay(1000);
    
    // Initialize logging system FIRST
    logger_init();
    
    // Initialize error management system
    // error_manager_init();  // Temporarily disabled
    
    LOG_SYSTEM_STARTUP();
    LOG_INFO(LOG_CAT_SYSTEM, "Initialization starting...");
    
    // #ifdef ENABLE_UNIT_TESTS
    // LOG_INFO(LOG_CAT_SYSTEM, "Running unit tests...");
    // test_run_all();
    // #endif
    // Note: Unit tests temporarily disabled for compilation
    
    // Initialize maritime-specific hardware
    setup_maritime_hardware();
    
    // Load maritime configuration
    load_maritime_config();
    
    // Initialize GNSS
    if (maritime_config.gnss_required) {
        setup_gnss();
    }
    
    // Initialize environmental sensors
    setup_maritime_sensors();
    
    // Initialize parameters system
    parameters.init();
    
    // Initialize LED system
    led_init();
    led_set_color(LED_COLOR_BLUE);  // Maritime mode indicator
    
    // Initialize WiFi
    setup_wifi();
    
    // Initialize MQTT if enabled
    if (maritime_config.mqtt_enabled) {
        setup_mqtt();
    }
    
    // Initialize web interface
    webif.init();
    
    // Initialize transmission systems
    wifi.init();
    ble.init();
    
#if AP_DRONECAN_ENABLED
    dronecan.init();
#endif

#if AP_MAVLINK_ENABLED
    mavlink1.init();
    mavlink2.init();
#endif
    
    // Maritime startup sequence
    maritime_startup_sequence();
    
    Serial.println("OndOcean RemoteID Maritime - Ready");
    led_set_color(LED_COLOR_GREEN);
}

void setup_maritime_hardware() {
    // Configure maritime-specific pins
    pinMode(PIN_GNSS_ANT_PWR, OUTPUT);
    digitalWrite(PIN_GNSS_ANT_PWR, HIGH);  // Enable GNSS antenna power
    
    pinMode(PIN_WIFI_ANT_SEL, OUTPUT);
    digitalWrite(PIN_WIFI_ANT_SEL, HIGH);  // Select external antenna
    
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);
    
    pinMode(PIN_STATUS_LED, OUTPUT);
    digitalWrite(PIN_STATUS_LED, HIGH);
    
    pinMode(PIN_POWER_ENABLE, OUTPUT);
    digitalWrite(PIN_POWER_ENABLE, HIGH);  // Ensure power enabled
    
    pinMode(PIN_CASE_DETECT, INPUT_PULLUP);
    
    // Configure I2C for sensors
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    
    Serial.println("Maritime hardware initialized");
}

void setup_gnss() {
    gnssSerial.begin(9600, SERIAL_8N1, PIN_GNSS_RX, PIN_GNSS_TX);
    
    // Configure u-blox for maritime use
    // Send UBX configuration commands for optimal maritime performance
    delay(1000);
    
    Serial.println("GNSS initialized for maritime use");
}

void setup_wifi() {
    // Maritime WiFi configuration
    WiFi.mode(WIFI_AP_STA);
    
    // Create maritime access point
    String ap_ssid = "ONRID-MAR-" + WiFi.macAddress().substring(9);
    ap_ssid.replace(":", "");
    
    WiFi.softAP(ap_ssid.c_str(), "OndOcean2025");
    
    Serial.println("WiFi configured: " + ap_ssid);
}

void setup_mqtt() {
    mqttClient.setServer(maritime_config.mqtt_broker.c_str(), maritime_config.mqtt_port);
    mqttClient.setCallback(mqtt_callback);
    
    // Generate device ID from MAC
    maritime_config.device_id = "ONRID-" + WiFi.macAddress();
    maritime_config.device_id.replace(":", "");
    
    Serial.println("MQTT configured: " + maritime_config.mqtt_broker);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    // Handle MQTT commands for maritime operations
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    
    Serial.println("MQTT received: " + String(topic) + " = " + message);
    
    // Process maritime-specific commands
    if (String(topic).endsWith("/command")) {
        process_maritime_command(message);
    }
}

void process_maritime_command(String command) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, command);
    
    if (doc["action"] == "emergency_beacon") {
        activate_emergency_beacon();
    } else if (doc["action"] == "low_power") {
        maritime_config.low_power_mode = doc["enabled"];
    } else if (doc["action"] == "update_position") {
        maritime_config.latitude = doc["lat"];
        maritime_config.longitude = doc["lon"];
        maritime_config.position_valid = true;
    }
}

void activate_emergency_beacon() {
    // Maritime emergency beacon
    led_set_color(LED_COLOR_RED);
    
    for (int i = 0; i < 5; i++) {
        digitalWrite(PIN_BUZZER, HIGH);
        delay(200);
        digitalWrite(PIN_BUZZER, LOW);
        delay(200);
    }
    
    // Increase transmission power and frequency
    // TODO: Implement emergency mode for WiFi and BLE transmitters
    // wifi.set_emergency_mode(true);
    // ble.set_emergency_mode(true);
    
    Serial.println("Maritime emergency beacon activated");
}

void maritime_startup_sequence() {
    Serial.println("Maritime startup sequence...");
    
    // Check case integrity
    maritime_config.case_closed = !digitalRead(PIN_CASE_DETECT);
    maritime_config.waterproof_sealed = maritime_config.case_closed;
    
    // LED startup pattern
    for (int i = 0; i < 3; i++) {
        led_set_color(LED_COLOR_BLUE);
        delay(200);
        led_set_color(LED_COLOR_OFF);
        delay(200);
    }
    
    // Buzzer confirmation
    digitalWrite(PIN_BUZZER, HIGH);
    delay(100);
    digitalWrite(PIN_BUZZER, LOW);
    
    Serial.println("Maritime startup sequence completed");
}

void loop() {
    static uint32_t last_update_ms = 0;
    uint32_t now_ms = millis();
    
    // Update at 10Hz
    if (now_ms - last_update_ms >= 100) {
        last_update_ms = now_ms;
        
        // Read maritime sensors
        update_maritime_sensors();
        
        // Read GNSS data
        if (maritime_config.gnss_required) {
            update_gnss_data();
        }
        
        // Update RemoteID data structure
        update_remoteid_data();
        
        // Transmit RemoteID
        transmit_remoteid();
        
        // MQTT publishing (1Hz)
        if (maritime_config.mqtt_enabled && (now_ms - last_mqtt_publish_ms >= 1000)) {
            publish_mqtt_data();
            last_mqtt_publish_ms = now_ms;
        }
        
        // Update status LED
        update_status_led();
    }
    
    // Handle MQTT
    if (maritime_config.mqtt_enabled) {
        if (!mqttClient.connected()) {
            reconnect_mqtt();
        }
        mqttClient.loop();
    }
    
    // Handle web interface
    webif.update();
    
    // Handle transport protocols
#if AP_DRONECAN_ENABLED
    dronecan.update();
#endif

#if AP_MAVLINK_ENABLED
    mavlink1.update();
    mavlink2.update();
#endif
    
    // Maritime-specific monitoring
    monitor_maritime_systems();
    
    // Power management
    if (maritime_config.low_power_mode) {
        handle_low_power_mode();
    }
}

void update_maritime_sensors() {
    // Read environmental sensors
    maritime_config.temperature = read_temperature();
    maritime_config.humidity = read_humidity();
    maritime_config.pressure = read_pressure();
    
    // Read battery voltage
    maritime_config.battery_voltage = read_battery_voltage();
    
    // Check case status
    maritime_config.case_closed = !digitalRead(PIN_CASE_DETECT);
}

void update_gnss_data() {
    while (gnssSerial.available()) {
        char c = gnssSerial.read();
        // Process NMEA sentences
        // Parse GPS data and update position
        // This would use a proper NMEA parser in production
    }
}

void update_remoteid_data() {
    // Update OpenDroneID data structure with maritime information
    UAS_data.BasicID[0].UAType = ODID_UATYPE_HELICOPTER_OR_MULTIROTOR;
    UAS_data.BasicID[0].IDType = ODID_IDTYPE_SERIAL_NUMBER;
    
    // Use maritime device ID
    strncpy((char*)UAS_data.BasicID[0].UASID, maritime_config.device_id.c_str(), 
            sizeof(UAS_data.BasicID[0].UASID));
    
    // Location data
    if (maritime_config.position_valid) {
        UAS_data.Location.Status = ODID_STATUS_AIRBORNE;  // Maritime equivalent
        UAS_data.Location.Latitude = maritime_config.latitude;
        UAS_data.Location.Longitude = maritime_config.longitude;
        UAS_data.Location.AltitudeBaro = maritime_config.altitude;
        UAS_data.Location.Height = 0;  // Sea level reference
        UAS_data.Location.HorizAccuracy = ODID_HOR_ACC_3_METER;
        UAS_data.Location.VertAccuracy = ODID_VER_ACC_3_METER;
        UAS_data.Location.TimeStamp = (millis() / 100) % 3600;  // Deciseconds in hour
    }
    
    // System data with maritime info
    UAS_data.System.OperatorLatitude = maritime_config.latitude;
    UAS_data.System.OperatorLongitude = maritime_config.longitude;
    UAS_data.System.AreaCount = 1;
    UAS_data.System.AreaRadius = 100;  // 100m maritime operation area
    UAS_data.System.AreaCeiling = 50;   // 50m max altitude for maritime
    UAS_data.System.AreaFloor = 0;
    UAS_data.System.CategoryEU = ODID_CATEGORY_EU_OPEN;
    UAS_data.System.ClassEU = ODID_CLASS_EU_CLASS_1;
}

void transmit_remoteid() {
    // Transmit via WiFi
    wifi.transmit_beacon(UAS_data);
    wifi.transmit_nan(UAS_data);
    
    // Transmit via BLE
    ble.transmit_legacy(UAS_data);
    ble.transmit_longrange(UAS_data);
}

void publish_mqtt_data() {
    if (!mqttClient.connected()) return;
    
    DynamicJsonDocument doc(2048);
    
    // Header according to OndOcean schema
    doc["header"]["timestamp"] = get_iso_timestamp();
    doc["header"]["device_id"] = maritime_config.device_id;
    doc["header"]["device_type"] = "remoteid";
    doc["header"]["firmware_version"] = "1.0.0-maritime";
    
    if (maritime_config.position_valid) {
        doc["header"]["location"]["latitude"] = maritime_config.latitude;
        doc["header"]["location"]["longitude"] = maritime_config.longitude;
        doc["header"]["location"]["altitude_m"] = maritime_config.altitude;
        doc["header"]["location"]["accuracy_m"] = maritime_config.accuracy;
        doc["header"]["location"]["source"] = "gnss";
    }
    
    // Data RemoteID
    doc["data"]["uas_id"] = maritime_config.device_id;
    doc["data"]["uas_type"] = ODID_UATYPE_HELICOPTER_OR_MULTIROTOR;
    doc["data"]["transmission_method"] = "wifi_beacon";
    doc["data"]["maritime_mode"] = true;
    
    if (maritime_config.position_valid) {
        doc["data"]["aircraft_location"]["latitude"] = maritime_config.latitude;
        doc["data"]["aircraft_location"]["longitude"] = maritime_config.longitude;
        doc["data"]["aircraft_location"]["altitude_m"] = maritime_config.altitude;
    }
    
    // Quality metrics
    doc["quality"]["signal_strength_dbm"] = -30;  // Strong signal
    doc["quality"]["confidence"] = maritime_config.position_valid ? 0.95 : 0.5;
    
    // Maritime sensors
    doc["maritime"]["temperature_c"] = maritime_config.temperature;
    doc["maritime"]["humidity_percent"] = maritime_config.humidity;
    doc["maritime"]["pressure_hpa"] = maritime_config.pressure;
    doc["maritime"]["battery_voltage"] = maritime_config.battery_voltage;
    doc["maritime"]["case_sealed"] = maritime_config.waterproof_sealed;
    
    String payload;
    serializeJson(doc, payload);
    
    String topic = maritime_config.mqtt_topic_prefix + "/data";
    mqttClient.publish(topic.c_str(), payload.c_str());
    
    Serial.println("MQTT published: " + String(payload.length()) + " bytes");
}

void reconnect_mqtt() {
    if (mqttClient.connect(maritime_config.device_id.c_str())) {
        Serial.println("MQTT reconnected");
        
        // Subscribe to command topic
        String cmd_topic = maritime_config.mqtt_topic_prefix + "/command";
        mqttClient.subscribe(cmd_topic.c_str());
        
        led_set_color(LED_COLOR_GREEN);
    } else {
        led_set_color(LED_COLOR_YELLOW);
    }
}

void update_status_led() {
    static uint32_t last_blink = 0;
    static bool blink_state = false;
    
    if (millis() - last_blink > 1000) {
        last_blink = millis();
        blink_state = !blink_state;
        
        if (!maritime_config.position_valid) {
            // Blink red if no GPS
            led_set_color(blink_state ? LED_COLOR_RED : LED_COLOR_OFF);
        } else if (!mqttClient.connected()) {
            // Blink yellow if no MQTT
            led_set_color(blink_state ? LED_COLOR_YELLOW : LED_COLOR_OFF);
        } else {
            // Solid green if all OK
            led_set_color(LED_COLOR_GREEN);
        }
    }
}

void monitor_maritime_systems() {
    // Monitor battery level
    if (maritime_config.battery_voltage < 3.3) {
        maritime_config.low_power_mode = true;
        Serial.println("Low battery - Low power mode activated");
    }
    
    // Monitor case integrity
    bool case_open = digitalRead(PIN_CASE_DETECT);
    if (case_open && maritime_config.case_closed) {
        Serial.println("ALERT: Case open!");
        maritime_config.waterproof_sealed = false;
        
        // Sound alarm
        for (int i = 0; i < 3; i++) {
            digitalWrite(PIN_BUZZER, HIGH);
            delay(100);
            digitalWrite(PIN_BUZZER, LOW);
            delay(100);
        }
    }
    maritime_config.case_closed = !case_open;
}

void handle_low_power_mode() {
    // Reduce transmission frequency
    static uint32_t last_low_power_action = 0;
    
    if (millis() - last_low_power_action > 5000) {  // Every 5 seconds in low power
        last_low_power_action = millis();
        
        // Reduce WiFi power
        esp_wifi_set_max_tx_power(10);  // Reduce to 2.5dBm
        
        // Sleep between transmissions
        delay(100);
    }
}

// Utility functions
String get_iso_timestamp() {
    time_t now;
    time(&now);
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    return String(buffer);
}

float read_temperature() {
    // Read from maritime sensor (BME280, SHT30, etc.)
    return 20.0;  // Placeholder
}

float read_humidity() {
    return 65.0;  // Placeholder
}

float read_pressure() {
    return 1013.25;  // Placeholder
}

// read_battery_voltage is defined in maritime_sensors.cpp

void load_maritime_config() {
    // Load configuration from EEPROM/NVS
    // For MVP, use defaults
    Serial.println("Maritime configuration loaded");
}

// Maritime sensors setup is now in maritime_sensors.cpp
