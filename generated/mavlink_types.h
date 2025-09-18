/*
 * MAVLink types header for OndOcean RemoteID
 * Simplified version for compilation
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

// MAVLink constants
#define MAVLINK_MAX_PAYLOAD_LEN 255
#define MAVLINK_CORE_HEADER_LEN 9
#define MAVLINK_CORE_HEADER_MAVLINK1_LEN 5
#define MAVLINK_NUM_HEADER_BYTES (MAVLINK_CORE_HEADER_LEN + 1)
#define MAVLINK_NUM_CHECKSUM_BYTES 2
#define MAVLINK_NUM_NON_PAYLOAD_BYTES (MAVLINK_NUM_HEADER_BYTES + MAVLINK_NUM_CHECKSUM_BYTES)
#define MAVLINK_SIGNATURE_BLOCK_LEN 13

#define MAVLINK_STX 0xFD
#define MAVLINK_STX_MAVLINK1 0xFE

// Parse states (must be defined first)
typedef enum {
    MAVLINK_PARSE_STATE_UNINIT=0,
    MAVLINK_PARSE_STATE_IDLE,
    MAVLINK_PARSE_STATE_GOT_STX,
    MAVLINK_PARSE_STATE_GOT_LENGTH,
    MAVLINK_PARSE_STATE_GOT_INCOMPAT_FLAGS,
    MAVLINK_PARSE_STATE_GOT_COMPAT_FLAGS,
    MAVLINK_PARSE_STATE_GOT_SEQ,
    MAVLINK_PARSE_STATE_GOT_SYSID,
    MAVLINK_PARSE_STATE_GOT_COMPID,
    MAVLINK_PARSE_STATE_GOT_MSGID1,
    MAVLINK_PARSE_STATE_GOT_MSGID2,
    MAVLINK_PARSE_STATE_GOT_MSGID3,
    MAVLINK_PARSE_STATE_GOT_PAYLOAD,
    MAVLINK_PARSE_STATE_GOT_CRC1,
    MAVLINK_PARSE_STATE_GOT_BAD_CRC1,
    MAVLINK_PARSE_STATE_SIGNATURE_WAIT
} mavlink_parse_state_t;

// MAVLink message structure
typedef struct __mavlink_message {
    uint16_t checksum;      // sent at end of packet
    uint8_t magic;          // protocol magic marker
    uint8_t len;            // Length of payload
    uint8_t incompat_flags; // flags that must be understood
    uint8_t compat_flags;   // flags that can be ignored if not understood
    uint8_t seq;            // Sequence of packet
    uint8_t sysid;          // ID of message sender system/aircraft
    uint8_t compid;         // ID of the message sender component
    uint32_t msgid:24;      // ID of message in payload
    uint64_t payload64[(MAVLINK_MAX_PAYLOAD_LEN+MAVLINK_NUM_CHECKSUM_BYTES+7)/8];
    uint8_t ck[2];          // incoming checksum bytes
    uint8_t signature[MAVLINK_SIGNATURE_BLOCK_LEN];
} mavlink_message_t;

// MAVLink status structure
typedef struct __mavlink_status {
    uint8_t msg_received;               // Number of received messages
    uint8_t buffer_overrun;             // Number of buffer overruns
    uint8_t parse_error;                // Number of parse errors
    mavlink_parse_state_t parse_state;  // Parsing state machine
    uint8_t packet_idx;                 // Index in current packet
    uint8_t current_rx_seq;             // Sequence number of last packet received
    uint8_t current_tx_seq;             // Sequence number of last packet sent
    uint16_t packet_rx_success_count;   // Received packets
    uint16_t packet_rx_drop_count;      // Number of packet drops
} mavlink_status_t;

// MAVLink system structure
typedef struct __mavlink_system {
    uint8_t sysid;   // System ID
    uint8_t compid;  // Component ID
} mavlink_system_t;
