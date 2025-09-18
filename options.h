/*
  control optional behaviour in the firmware at build time
 */
#pragma once

#include "board_config.h"

// do we support DroneCAN connnection to flight controller?
// Temporarily disabled for initial compilation
#define AP_DRONECAN_ENABLED 0

// do we support MAVLink connnection to flight controller?
#define AP_MAVLINK_ENABLED 1
