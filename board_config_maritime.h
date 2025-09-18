/*
 * OndOcean Maritime Board Configuration
 * ESP32-S3 pin definitions for maritime RemoteID module
 * Extends base ArduRemoteID configuration with maritime-specific features
 */

#ifndef BOARD_CONFIG_MARITIME_H
#define BOARD_CONFIG_MARITIME_H

#include <Arduino.h>

// Define maritime board variant BEFORE including base config
#define BOARD_ONDOCEAN_MARITIME
#ifndef BOARD_ESP32S3_DEV
#define BOARD_ESP32S3_DEV  // Use ESP32-S3 base configuration
#endif

// Include base board configuration
#include "board_config.h"

// Override board ID for maritime variant
#undef BOARD_ID
#define BOARD_ID 10        // Maritime-specific board ID

// Maritime-specific GPIO assignments (additional to base config)
#define PIN_GNSS_TX         GPIO_NUM_43  // u-blox F9P UART TX
#define PIN_GNSS_RX         GPIO_NUM_44  // u-blox F9P UART RX
#define PIN_GNSS_PPS        GPIO_NUM_45  // Pulse Per Second from GNSS
#define PIN_GNSS_ANT_PWR    GPIO_NUM_46  // GNSS antenna power control

// I2C for maritime sensors
#define PIN_I2C_SDA         21  // I2C Data
#define PIN_I2C_SCL         22  // I2C Clock
#define I2C_FREQUENCY       400000       // 400kHz I2C

// Power management
#define PIN_POWER_ENABLE    GPIO_NUM_2   // PoE splitter enable
#define PIN_STATUS_LED      GPIO_NUM_1   // Status LED (separate from WS2812)
#define PIN_BATTERY_MONITOR GPIO_NUM_4   // Battery voltage monitoring (ADC)

// IP67 case detection
#define PIN_CASE_DETECT     GPIO_NUM_39  // Case closure detection

// Audio feedback (optional buzzer)
#define PIN_BUZZER          GPIO_NUM_3   // Buzzer for audio alerts

// WiFi antenna selection (if applicable)
#define PIN_WIFI_ANT_SEL    GPIO_NUM_0   // WiFi antenna selector

// Maritime-specific features
#define MARITIME_MODE_ENABLED 1
#define GNSS_REQUIRED 1
#define WATERPROOF_CASE 1

#define CAN_APP_NODE_NAME "OndOcean RemoteID Maritime"

#endif // BOARD_CONFIG_MARITIME_H
