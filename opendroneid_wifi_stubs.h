/*
 * OpenDroneID WiFi Stubs Header for OndOcean RemoteID
 * 
 * Function declarations for OpenDroneID WiFi stub implementations
 */

#pragma once

#include <stdint.h>
#include "opendroneid.h"

#ifdef __cplusplus
extern "C" {
#endif

// WiFi beacon frame functions (signatures from opendroneid.h)
int odid_wifi_build_message_pack_beacon_frame(ODID_UAS_Data *UAS_Data, 
                                               char *mac,
                                               const char *SSID, 
                                               size_t SSID_len,
                                               uint16_t interval_tu,
                                               uint8_t send_counter,
                                               uint8_t *buf, 
                                               size_t buf_size);

// NAN (Neighbor Aware Networking) functions (signatures from opendroneid.h)
int odid_wifi_build_nan_sync_beacon_frame(char *mac, 
                                           uint8_t *buf, 
                                           size_t buf_size);

int odid_wifi_build_message_pack_nan_action_frame(ODID_UAS_Data *UAS_Data,
                                                   char *mac,
                                                   uint8_t send_counter,
                                                   uint8_t *buf,
                                                   size_t buf_size);

// Note: odid_message_build_pack is already declared in opendroneid.h

// Note: OpenDroneID encoding functions are already declared in opendroneid.h
// We only provide implementations with correct signatures

// WiFi transmission control
int odid_wifi_init(void);
int odid_wifi_transmit_beacon(uint8_t *beacon_frame, int beacon_length);
int odid_wifi_transmit_nan(uint8_t *nan_frame, int nan_length);

#ifdef __cplusplus
}
#endif
