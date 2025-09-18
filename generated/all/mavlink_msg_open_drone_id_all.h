/*
 * MAVLink OpenDroneID All Messages for OndOcean RemoteID
 * Simplified version for compilation
 */

#pragma once

#include "../mavlink_types.h"
#include "../mavlink_helpers.h"

// OpenDroneID message IDs
#define MAVLINK_MSG_ID_OPEN_DRONE_ID_LOCATION 12901

// OpenDroneID ARM status constants
#define MAV_ODID_ARM_STATUS_GOOD_TO_ARM 0
#define MAV_ODID_ARM_STATUS_PRE_ARM_FAIL_GENERIC 1
#define MAVLINK_MSG_ID_OPEN_DRONE_ID_AUTHENTICATION 12902
#define MAVLINK_MSG_ID_OPEN_DRONE_ID_SELF_ID 12903
#define MAVLINK_MSG_ID_OPEN_DRONE_ID_SYSTEM 12904
#define MAVLINK_MSG_ID_OPEN_DRONE_ID_OPERATOR_ID 12905
#define MAVLINK_MSG_ID_OPEN_DRONE_ID_MESSAGE_PACK 12915
#define MAVLINK_MSG_ID_OPEN_DRONE_ID_ARM_STATUS 12918
#define MAVLINK_MSG_ID_SECURE_COMMAND 76
#define MAVLINK_MSG_ID_SECURE_COMMAND_REPLY 77
#define MAVLINK_MSG_ID_PARAM_REQUEST_LIST 21
#define MAVLINK_MSG_ID_PARAM_REQUEST_READ 20
#define MAVLINK_MSG_ID_PARAM_VALUE 22
#define MAVLINK_MSG_ID_PARAM_SET 23
#define MAVLINK_MSG_ID_OPEN_DRONE_ID_SYSTEM_UPDATE 12919
#define MAVLINK_MSG_ID_HEARTBEAT 0
#define MAVLINK_MSG_ID_OPEN_DRONE_ID_BASIC_ID 12900

// MAVLink types
#define MAV_TYPE_GCS 6
#define MAV_TYPE_ODID 18

// MAVLink autopilot types
#define MAV_AUTOPILOT_INVALID 8

// MAVLink component IDs
#define MAV_COMP_ID_ODID_TXRX_1 236

// MAVLink message field lengths
#define MAVLINK_MSG_STATUSTEXT_FIELD_TEXT_LEN 50

// OpenDroneID ID types
#define MAV_ODID_ID_TYPE_SPECIFIC_SESSION_ID 4

// Channel type
typedef enum {
    MAVLINK_COMM_0 = 0,
    MAVLINK_COMM_1 = 1,
    MAVLINK_COMM_2 = 2,
    MAVLINK_COMM_3 = 3
} mavlink_channel_t;

// Message structures (stubs)
typedef struct __mavlink_open_drone_id_location_t {
    uint8_t target_system;
    uint8_t target_component;
    int32_t latitude;
    int32_t longitude;
    float altitude_barometric;
    float altitude_geodetic;
    uint16_t height;
    uint16_t horizontal_accuracy;
    uint16_t vertical_accuracy;
    uint16_t barometer_accuracy;
    uint16_t speed_accuracy;
    uint64_t timestamp;
    uint8_t direction;
    uint8_t speed_horizontal;
    int8_t speed_vertical;
} mavlink_open_drone_id_location_t;

typedef struct __mavlink_open_drone_id_authentication_t {
    uint8_t target_system;
    uint8_t target_component;
    uint8_t id_or_mac[20];
    uint8_t authentication_type;
    uint8_t data_page;
    uint8_t last_page_index;
    uint8_t length;
    uint32_t timestamp;
    uint8_t authentication_data[17];
} mavlink_open_drone_id_authentication_t;

typedef struct __mavlink_open_drone_id_self_id_t {
    uint8_t target_system;
    uint8_t target_component;
    uint8_t id_or_mac[20];
    uint8_t description_type;
    char description[23];
} mavlink_open_drone_id_self_id_t;

typedef struct __mavlink_open_drone_id_system_t {
    uint8_t target_system;
    uint8_t target_component;
    uint8_t id_or_mac[20];
    uint8_t operator_location_type;
    uint8_t classification_type;
    int32_t operator_latitude;
    int32_t operator_longitude;
    uint16_t area_count;
    uint16_t area_radius;
    float area_ceiling;
    float area_floor;
    uint8_t category_eu;
    uint8_t class_eu;
    float operator_altitude_geo;
    uint32_t timestamp;
} mavlink_open_drone_id_system_t;

typedef struct __mavlink_open_drone_id_operator_id_t {
    uint8_t target_system;
    uint8_t target_component;
    uint8_t id_or_mac[20];
    uint8_t operator_id_type;
    char operator_id[20];
} mavlink_open_drone_id_operator_id_t;

typedef struct __mavlink_open_drone_id_message_pack_t {
    uint8_t target_system;
    uint8_t target_component;
    uint8_t id_or_mac[20];
    uint8_t single_message_size;
    uint8_t msg_pack_size;
    uint8_t messages[225];
} mavlink_open_drone_id_message_pack_t;

typedef struct __mavlink_open_drone_id_arm_status_t {
    uint8_t status;
    char error[50];
} mavlink_open_drone_id_arm_status_t;

typedef struct __mavlink_open_drone_id_basic_id_t {
    uint8_t target_system;
    uint8_t target_component;
    uint8_t id_or_mac[20];
    uint8_t id_type;
    uint8_t ua_type;
    char uas_id[20];
} mavlink_open_drone_id_basic_id_t;

typedef struct __mavlink_open_drone_id_system_update_t {
    uint8_t target_system;
    uint8_t target_component;
    uint8_t id_or_mac[20];
    int32_t operator_latitude;
    int32_t operator_longitude;
    float operator_altitude_geo;
    uint32_t timestamp;
} mavlink_open_drone_id_system_update_t;

// Function stubs for all OpenDroneID messages
static inline void mavlink_msg_open_drone_id_location_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, const uint8_t *id_or_mac, uint8_t status, uint16_t direction, uint16_t speed_horizontal, int16_t speed_vertical, int32_t latitude, int32_t longitude, float altitude_barometric, float altitude_geodetic, uint16_t height, uint16_t horizontal_accuracy, uint16_t vertical_accuracy, uint16_t barometer_accuracy, uint16_t speed_accuracy, uint64_t timestamp) {
    // Stub implementation
}

static inline void mavlink_msg_open_drone_id_authentication_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, const uint8_t *id_or_mac, uint8_t authentication_type, uint8_t data_page, uint8_t last_page_index, uint8_t length, uint32_t timestamp, const uint8_t *authentication_data) {
    // Stub implementation
}

static inline void mavlink_msg_open_drone_id_self_id_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, const uint8_t *id_or_mac, uint8_t description_type, const char *description) {
    // Stub implementation
}

static inline void mavlink_msg_open_drone_id_system_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, const uint8_t *id_or_mac, uint8_t operator_location_type, uint8_t classification_type, int32_t operator_latitude, int32_t operator_longitude, uint16_t area_count, uint16_t area_radius, float area_ceiling, float area_floor, uint8_t category_eu, uint8_t class_eu, float operator_altitude_geo, uint32_t timestamp) {
    // Stub implementation
}

static inline void mavlink_msg_open_drone_id_operator_id_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, const uint8_t *id_or_mac, uint8_t operator_id_type, const char *operator_id) {
    // Stub implementation
}

static inline void mavlink_msg_open_drone_id_message_pack_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, const uint8_t *id_or_mac, uint8_t single_message_size, uint8_t msg_pack_size, const uint8_t *messages) {
    // Stub implementation
}

static inline void mavlink_msg_open_drone_id_arm_status_send(mavlink_channel_t chan, uint8_t status, const char *error) {
    // Stub implementation
}

// OpenDroneID decode functions (stubs)
static inline void mavlink_msg_open_drone_id_location_decode(const mavlink_message_t* msg, mavlink_open_drone_id_location_t* location) {
    // Stub implementation
}

static inline void mavlink_msg_open_drone_id_authentication_decode(const mavlink_message_t* msg, mavlink_open_drone_id_authentication_t* authentication) {
    // Stub implementation
}

static inline void mavlink_msg_open_drone_id_self_id_decode(const mavlink_message_t* msg, mavlink_open_drone_id_self_id_t* self_id) {
    // Stub implementation
}

static inline void mavlink_msg_open_drone_id_system_decode(const mavlink_message_t* msg, mavlink_open_drone_id_system_t* system) {
    // Stub implementation
}

static inline void mavlink_msg_open_drone_id_operator_id_decode(const mavlink_message_t* msg, mavlink_open_drone_id_operator_id_t* operator_id) {
    // Stub implementation
}

static inline void mavlink_msg_open_drone_id_basic_id_decode(const mavlink_message_t* msg, mavlink_open_drone_id_basic_id_t* basic_id) {
    // Stub implementation
}

static inline void mavlink_msg_open_drone_id_system_update_decode(const mavlink_message_t* msg, mavlink_open_drone_id_system_update_t* system_update) {
    // Stub implementation
}

// Secure command message structure
typedef struct __mavlink_secure_command_t {
    uint8_t target_system;
    uint8_t target_component;
    uint32_t sequence;
    uint32_t command;
    uint32_t operation;
    uint8_t sig_length;
    uint8_t data[220];
    uint8_t data_length;
} mavlink_secure_command_t;

// Secure command reply structure
typedef struct __mavlink_secure_command_reply_t {
    uint32_t sequence;
    uint32_t operation;
    uint8_t result;
    uint8_t data[220];
    uint8_t data_length;
} mavlink_secure_command_reply_t;

// MAVLink result constants
#define MAV_RESULT_ACCEPTED 0
#define MAV_RESULT_FAILED 4
#define MAV_RESULT_DENIED 6
#define MAV_RESULT_UNSUPPORTED 3

// MAVLink severity constants
#define MAV_SEVERITY_INFO 6
#define MAV_SEVERITY_ERROR 3

// MAVLink parameter types
#define MAV_PARAM_TYPE_REAL32 9

// Secure command types
#define SECURE_COMMAND_GET_PUBLIC_KEYS 1
#define SECURE_COMMAND_SET_PUBLIC_KEYS 2
#define SECURE_COMMAND_REMOVE_PUBLIC_KEYS 3
#define SECURE_COMMAND_SET_REMOTEID_CONFIG 4
#define SECURE_COMMAND_GET_SESSION_KEY 5
#define SECURE_COMMAND_GET_REMOTEID_SESSION_KEY 6

// Secure command functions (stubs)
static inline void mavlink_msg_secure_command_decode(const mavlink_message_t* msg, mavlink_secure_command_t* secure_command) {
    // Stub implementation
}

static inline void mavlink_msg_secure_command_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint32_t sequence, uint32_t command, const uint8_t *data) {
    // Stub implementation
}

static inline void mavlink_msg_secure_command_reply_send_struct(mavlink_channel_t chan, const mavlink_secure_command_reply_t* reply) {
    // Stub implementation
}

// Printf function stub
static inline void mav_printf(uint8_t severity, const char* format, ...) {
    // Stub implementation - could forward to Serial.printf in real implementation
}

// Parameter value message
static inline void mavlink_msg_param_value_send(mavlink_channel_t chan, const char* param_id, float param_value, uint8_t param_type, uint16_t param_count, uint16_t param_index) {
    // Stub implementation
}

// Parameter set message structure
typedef struct __mavlink_param_set_t {
    uint8_t target_system;
    uint8_t target_component;
    char param_id[16];
    float param_value;
    uint8_t param_type;
} mavlink_param_set_t;

// Parameter set decode function
static inline void mavlink_msg_param_set_decode(const mavlink_message_t* msg, mavlink_param_set_t* param_set) {
    // Stub implementation
}

// Parameter request read message structure
typedef struct __mavlink_param_request_read_t {
    uint8_t target_system;
    uint8_t target_component;
    char param_id[16];
    int16_t param_index;
} mavlink_param_request_read_t;

// Parameter request read decode function
static inline void mavlink_msg_param_request_read_decode(const mavlink_message_t* msg, mavlink_param_request_read_t* param_request_read) {
    // Stub implementation
}

// Heartbeat message structure
typedef struct __mavlink_heartbeat_t {
    uint8_t type;
    uint8_t autopilot;
    uint8_t base_mode;
    uint32_t custom_mode;
    uint8_t system_status;
    uint8_t mavlink_version;
} mavlink_heartbeat_t;

// Heartbeat decode function
static inline void mavlink_msg_heartbeat_decode(const mavlink_message_t* msg, mavlink_heartbeat_t* heartbeat) {
    // Stub implementation
}

// Heartbeat send function
static inline void mavlink_msg_heartbeat_send(mavlink_channel_t chan, uint8_t type, uint8_t autopilot, uint8_t base_mode, uint32_t custom_mode, uint8_t system_status) {
    // Stub implementation
}

// Status text send function
static inline void mavlink_msg_statustext_send(mavlink_channel_t chan, uint8_t severity, const char* text, uint16_t id, uint8_t chunk_seq) {
    // Stub implementation - extended signature for compatibility
}
