/*
 * MAVLink helpers header for OndOcean RemoteID
 * Simplified version for compilation
 */

#pragma once

#include "mavlink_types.h"
#include <stdint.h>
#include <string.h>

// MAVLink helper functions (stubs for compilation)
static inline uint16_t mavlink_finalize_message(mavlink_message_t* msg, uint8_t system_id, uint8_t component_id, uint8_t min_length, uint8_t length, uint8_t crc_extra) {
    return 0; // Stub implementation
}

static inline uint16_t mavlink_finalize_message_chan(mavlink_message_t* msg, uint8_t system_id, uint8_t component_id, uint8_t chan, uint8_t min_length, uint8_t length, uint8_t crc_extra) {
    return 0; // Stub implementation
}

static inline void mavlink_start_checksum(mavlink_message_t* msg) {
    // Stub implementation
}

static inline void mavlink_update_checksum(mavlink_message_t* msg, uint8_t c) {
    // Stub implementation
}

static inline uint8_t mavlink_frame_char(uint8_t chan, uint8_t c, mavlink_message_t* r_message, mavlink_status_t* r_mavlink_status) {
    return 0; // Stub implementation
}

// Message packing functions (stubs)
static inline uint16_t mavlink_msg_to_send_buffer(uint8_t *buffer, const mavlink_message_t *msg) {
    return 0; // Stub implementation
}

static inline void _mav_put_uint8_t(char *buf, uint8_t wire_offset, uint8_t b) {
    // Stub implementation
}

static inline void _mav_put_uint16_t(char *buf, uint8_t wire_offset, uint16_t b) {
    // Stub implementation
}

static inline void _mav_put_uint32_t(char *buf, uint8_t wire_offset, uint32_t b) {
    // Stub implementation
}

static inline void _mav_put_uint64_t(char *buf, uint8_t wire_offset, uint64_t b) {
    // Stub implementation
}

static inline void _mav_put_int8_t(char *buf, uint8_t wire_offset, int8_t b) {
    // Stub implementation
}

static inline void _mav_put_int16_t(char *buf, uint8_t wire_offset, int16_t b) {
    // Stub implementation
}

static inline void _mav_put_int32_t(char *buf, uint8_t wire_offset, int32_t b) {
    // Stub implementation
}

static inline void _mav_put_int64_t(char *buf, uint8_t wire_offset, int64_t b) {
    // Stub implementation
}

static inline void _mav_put_float(char *buf, uint8_t wire_offset, float b) {
    // Stub implementation
}

static inline void _mav_put_double(char *buf, uint8_t wire_offset, double b) {
    // Stub implementation
}

// Message extraction functions (stubs)
static inline uint8_t _MAV_RETURN_uint8_t(const mavlink_message_t *msg, uint8_t wire_offset) {
    return 0; // Stub implementation
}

static inline uint16_t _MAV_RETURN_uint16_t(const mavlink_message_t *msg, uint8_t wire_offset) {
    return 0; // Stub implementation
}

static inline uint32_t _MAV_RETURN_uint32_t(const mavlink_message_t *msg, uint8_t wire_offset) {
    return 0; // Stub implementation
}

static inline uint64_t _MAV_RETURN_uint64_t(const mavlink_message_t *msg, uint8_t wire_offset) {
    return 0; // Stub implementation
}

static inline int8_t _MAV_RETURN_int8_t(const mavlink_message_t *msg, uint8_t wire_offset) {
    return 0; // Stub implementation
}

static inline int16_t _MAV_RETURN_int16_t(const mavlink_message_t *msg, uint8_t wire_offset) {
    return 0; // Stub implementation
}

static inline int32_t _MAV_RETURN_int32_t(const mavlink_message_t *msg, uint8_t wire_offset) {
    return 0; // Stub implementation
}

static inline int64_t _MAV_RETURN_int64_t(const mavlink_message_t *msg, uint8_t wire_offset) {
    return 0; // Stub implementation
}

static inline float _MAV_RETURN_float(const mavlink_message_t *msg, uint8_t wire_offset) {
    return 0.0f; // Stub implementation
}

static inline double _MAV_RETURN_double(const mavlink_message_t *msg, uint8_t wire_offset) {
    return 0.0; // Stub implementation
}
