/*
 * MAVLink main header for OndOcean RemoteID
 * Simplified version for compilation
 */

#pragma once

#include <stdint.h>
#include <string.h>
#include "../mavlink_types.h"

// Basic MAVLink types (defined in mavlink_types.h)

// Basic MAVLink functions (stubs for compilation)
static inline uint16_t mavlink_msg_get_msgid(const mavlink_message_t* msg) {
    return msg->msgid;
}

static inline uint8_t mavlink_parse_char(uint8_t chan, uint8_t c, mavlink_message_t* r_message, mavlink_status_t* r_mavlink_status) {
    return 0; // Stub implementation
}

// OpenDroneID message IDs
#define MAVLINK_MSG_ID_OPEN_DRONE_ID_LOCATION 12901
#define MAVLINK_MSG_ID_OPEN_DRONE_ID_AUTHENTICATION 12902
#define MAVLINK_MSG_ID_OPEN_DRONE_ID_SELF_ID 12903
#define MAVLINK_MSG_ID_OPEN_DRONE_ID_SYSTEM 12904
#define MAVLINK_MSG_ID_OPEN_DRONE_ID_OPERATOR_ID 12905

// Include OpenDroneID message definitions
#include "mavlink_msg_open_drone_id_all.h"

// Global MAVLink system variable
extern mavlink_system_t mavlink_system;
