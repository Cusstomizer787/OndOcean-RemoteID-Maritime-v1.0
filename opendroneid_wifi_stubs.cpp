/*
 * OpenDroneID WiFi Stubs for OndOcean RemoteID
 * 
 * These are stub implementations of OpenDroneID WiFi functions
 * to enable successful compilation and linking.
 * 
 * For production use, replace with actual OpenDroneID library
 * or implement according to ASTM F3586-22 standard.
 */

#include <Arduino.h>
#include "opendroneid.h"

// Stub implementation for WiFi beacon frame building
// Signature from opendroneid.h: int odid_wifi_build_message_pack_beacon_frame(ODID_UAS_Data *UAS_Data, char *mac, const char *SSID, size_t SSID_len, uint16_t interval_tu, uint8_t send_counter, uint8_t *buf, size_t buf_size);
int odid_wifi_build_message_pack_beacon_frame(ODID_UAS_Data *UAS_Data, 
                                               char *mac,
                                               const char *SSID, 
                                               size_t SSID_len,
                                               uint16_t interval_tu,
                                               uint8_t send_counter,
                                               uint8_t *buf, 
                                               size_t buf_size)
{
    Serial.println("STUB: odid_wifi_build_message_pack_beacon_frame called");
    
    // Return minimal beacon frame for compilation
    if (buf && buf_size > 0) {
        // Minimal beacon frame structure
        memset(buf, 0, buf_size);
        
        // Add basic beacon frame headers (simplified)
        if (buf_size >= 2) {
            buf[0] = 0x80; // Frame control - beacon
            buf[1] = 0x00;
        }
        
        Serial.printf("STUB: Generated beacon frame, buffer size: %zu\n", buf_size);
        return buf_size > 128 ? 128 : buf_size; // Return used length
    }
    
    return ODID_FAIL;
}

// Stub implementation for NAN sync beacon frame
// Signature from opendroneid.h: int odid_wifi_build_nan_sync_beacon_frame(char *mac, uint8_t *buf, size_t buf_size);
int odid_wifi_build_nan_sync_beacon_frame(char *mac, 
                                           uint8_t *buf, 
                                           size_t buf_size)
{
    Serial.println("STUB: odid_wifi_build_nan_sync_beacon_frame called");
    
    if (buf && buf_size > 0) {
        // Minimal NAN sync beacon
        memset(buf, 0, buf_size);
        
        // Basic NAN beacon structure
        if (buf_size >= 2) {
            buf[0] = 0x80; // Frame control
            buf[1] = 0x00;
        }
        
        Serial.printf("STUB: Generated NAN sync beacon, buffer size: %zu\n", buf_size);
        return buf_size > 64 ? 64 : buf_size; // Return used length
    }
    
    return ODID_FAIL;
}

// Stub implementation for NAN action frame
// Signature from opendroneid.h: int odid_wifi_build_message_pack_nan_action_frame(ODID_UAS_Data *UAS_Data, char *mac, uint8_t send_counter, uint8_t *buf, size_t buf_size);
int odid_wifi_build_message_pack_nan_action_frame(ODID_UAS_Data *UAS_Data,
                                                   char *mac,
                                                   uint8_t send_counter,
                                                   uint8_t *buf,
                                                   size_t buf_size)
{
    Serial.println("STUB: odid_wifi_build_message_pack_nan_action_frame called");
    
    if (buf && buf_size > 0) {
        // Minimal NAN action frame
        memset(buf, 0, buf_size);
        
        // Basic action frame structure
        if (buf_size >= 2) {
            buf[0] = 0xD0; // Frame control - action
            buf[1] = 0x00;
        }
        
        Serial.printf("STUB: Generated NAN action frame, buffer size: %zu\n", buf_size);
        return buf_size > 256 ? 256 : buf_size; // Return used length
    }
    
    return ODID_FAIL;
}

// OpenDroneID encoding functions stubs (signatures from opendroneid.h)
int encodeLocationMessage(ODID_Location_encoded *outEncoded, ODID_Location_data *inData)
{
    Serial.println("STUB: encodeLocationMessage called");
    if (outEncoded && inData) {
        memset(outEncoded, 0, sizeof(ODID_Location_encoded));
        Serial.println("STUB: Location message encoded");
        return ODID_SUCCESS;
    }
    return ODID_FAIL;
}

int encodeBasicIDMessage(ODID_BasicID_encoded *outEncoded, ODID_BasicID_data *inData)
{
    Serial.println("STUB: encodeBasicIDMessage called");
    if (outEncoded && inData) {
        memset(outEncoded, 0, sizeof(ODID_BasicID_encoded));
        Serial.println("STUB: BasicID message encoded");
        return ODID_SUCCESS;
    }
    return ODID_FAIL;
}

int encodeSelfIDMessage(ODID_SelfID_encoded *outEncoded, ODID_SelfID_data *inData)
{
    Serial.println("STUB: encodeSelfIDMessage called");
    if (outEncoded && inData) {
        memset(outEncoded, 0, sizeof(ODID_SelfID_encoded));
        Serial.println("STUB: SelfID message encoded");
        return ODID_SUCCESS;
    }
    return ODID_FAIL;
}

int encodeSystemMessage(ODID_System_encoded *outEncoded, ODID_System_data *inData)
{
    Serial.println("STUB: encodeSystemMessage called");
    if (outEncoded && inData) {
        memset(outEncoded, 0, sizeof(ODID_System_encoded));
        Serial.println("STUB: System message encoded");
        return ODID_SUCCESS;
    }
    return ODID_FAIL;
}

int encodeOperatorIDMessage(ODID_OperatorID_encoded *outEncoded, ODID_OperatorID_data *inData)
{
    Serial.println("STUB: encodeOperatorIDMessage called");
    if (outEncoded && inData) {
        memset(outEncoded, 0, sizeof(ODID_OperatorID_encoded));
        Serial.println("STUB: OperatorID message encoded");
        return ODID_SUCCESS;
    }
    return ODID_FAIL;
}

// Implementation of odid_message_build_pack (declared in opendroneid.h)
int odid_message_build_pack(ODID_UAS_Data *UAS_Data, void *pack, size_t buflen)
{
    Serial.println("STUB: odid_message_build_pack called");
    
    if (pack && buflen > 0) {
        // Create minimal message pack
        memset(pack, 0, buflen);
        
        // Add basic OpenDroneID message structure
        if (buflen >= 25) {
            uint8_t *buf = (uint8_t*)pack;
            buf[0] = 0x0D; // OpenDroneID message type
            buf[1] = 0x00; // Reserved
            
            Serial.printf("STUB: Generated message pack, buffer size: %zu\n", buflen);
            return 25; // Return minimal message size
        }
    }
    
    return 0;
}

// WiFi transmission control stubs
int odid_wifi_init(void)
{
    Serial.println("STUB: odid_wifi_init called - WiFi OpenDroneID initialized");
    return ODID_SUCCESS;
}

int odid_wifi_transmit_beacon(uint8_t *beacon_frame, int beacon_length)
{
    Serial.printf("STUB: odid_wifi_transmit_beacon called - length: %d\n", beacon_length);
    return ODID_SUCCESS;
}

int odid_wifi_transmit_nan(uint8_t *nan_frame, int nan_length)
{
    Serial.printf("STUB: odid_wifi_transmit_nan called - length: %d\n", nan_length);
    return ODID_SUCCESS;
}
