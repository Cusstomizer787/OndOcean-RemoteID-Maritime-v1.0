#!/usr/bin/env python3
"""
OndOcéan RemoteID Maritime - Development Environment Setup
Prepares ESP32-S3 development environment and validates dependencies

IMPORTANT: Update the configuration paths below for your specific environment
before running this script.
"""

import os
import sys
import json
import shutil
import subprocess
from pathlib import Path

# Configuration - Update these paths for your environment
ARDUINO_REMOTEID_PATH = r"path\to\ArduRemoteID\RemoteIDModule"
ONDOCEAN_REMOTEID_PATH = r"path\to\ondocean-remoteid"
ARDUINO_CLI_PATH = r"arduino-cli"  # Use system PATH or full path

# Required ArduRemoteID files
REQUIRED_REMOTEID_FILES = [
    "options.h",
    "version.h", 
    "opendroneid.h",
    "mavlink.h",
    "DroneCAN.h",
    "WiFi_TX.h",
    "BLE_TX.h",
    "parameters.h",
    "webinterface.h",
    "check_firmware.h",
    "efuse.h",
    "led.h"
]

# ESP32-S3 Board Configuration
ESP32_CONFIG = {
    "board": "esp32s3",
    "variant": "esp32s3",
    "flash_size": "16MB",
    "psram": "enabled",
    "cpu_freq": "240MHz",
    "flash_freq": "80MHz",
    "partition_scheme": "app3M_fat9M_fact512k_16MB"
}

def check_arduino_cli():
    """Check if Arduino CLI is installed"""
    try:
        result = subprocess.run([ARDUINO_CLI_PATH, "version"], 
                              capture_output=True, text=True)
        if result.returncode == 0:
            print(f"Arduino CLI found: {result.stdout.strip()}")
            return True
    except FileNotFoundError:
        pass
    
    # Try system PATH
    try:
        result = subprocess.run(["arduino-cli", "version"], 
                              capture_output=True, text=True)
        if result.returncode == 0:
            print(f"Arduino CLI found in PATH: {result.stdout.strip()}")
            return True
    except FileNotFoundError:
        pass
    
    print("Arduino CLI not found")
    print("Install from: https://arduino.github.io/arduino-cli/")
    return False

def setup_arduino_environment():
    """Setup Arduino environment for ESP32-S3"""
    print("\nSetting up Arduino environment...")
    
    # Use full path to arduino-cli
    cli_cmd = ARDUINO_CLI_PATH if os.path.exists(ARDUINO_CLI_PATH) else "arduino-cli"
    
    commands = [
        [cli_cmd, "core", "update-index"],
        [cli_cmd, "core", "install", "esp32:esp32"],
        [cli_cmd, "lib", "install", "ArduinoJson"],
        [cli_cmd, "lib", "install", "PubSubClient"],
        [cli_cmd, "lib", "install", "WiFi"],
    ]
    
    for cmd in commands:
        print(f"Running: {' '.join(cmd)}")
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"Command failed: {result.stderr}")
            return False
        else:
            print(f"Success")
    
    return True

def copy_remoteid_dependencies():
    """Copy required files from ArduRemoteID"""
    print(f"\nCopying ArduRemoteID dependencies...")
    
    if not os.path.exists(ARDUINO_REMOTEID_PATH):
        print(f"ArduRemoteID path not found: {ARDUINO_REMOTEID_PATH}")
        return False
    
    copied_files = []
    missing_files = []
    
    for filename in REQUIRED_REMOTEID_FILES:
        src_path = os.path.join(ARDUINO_REMOTEID_PATH, filename)
        dst_path = os.path.join(ONDOCEAN_REMOTEID_PATH, filename)
        
        if os.path.exists(src_path):
            if not os.path.exists(dst_path):
                shutil.copy2(src_path, dst_path)
                copied_files.append(filename)
                print(f"Copied: {filename}")
            else:
                print(f"Already exists: {filename}")
        else:
            missing_files.append(filename)
            print(f"Missing: {filename}")
    
    if missing_files:
        print(f"\nMissing files: {missing_files}")
        print("Please check ArduRemoteID installation")
    
    return len(missing_files) == 0

def create_arduino_sketch_structure():
    """Create proper Arduino sketch structure"""
    print(f"\nCreating Arduino sketch structure...")
    
    # Create libraries directory
    lib_dir = os.path.join(ONDOCEAN_REMOTEID_PATH, "libraries")
    os.makedirs(lib_dir, exist_ok=True)
    
    # Create maritime library
    maritime_lib_dir = os.path.join(lib_dir, "OndOceanMaritime")
    os.makedirs(maritime_lib_dir, exist_ok=True)
    
    # Create library.properties
    lib_props = """name=OndOcean Maritime
version=1.0.0
author=OndOcean Team
maintainer=OndOcean <contact@ondocean.com>
sentence=Maritime adaptations for RemoteID
paragraph=Environmental sensors, MQTT telemetry, and maritime-specific features
category=Communication
url=https://github.com/ondocean/maritime-remoteid
architectures=esp32
includes=maritime_sensors.h,ondocean_mqtt.h
"""
    
    with open(os.path.join(maritime_lib_dir, "library.properties"), "w") as f:
        f.write(lib_props)
    
    # Move maritime headers to library
    src_dir = Path(ONDOCEAN_REMOTEID_PATH)
    dst_dir = Path(maritime_lib_dir)
    
    for header_file in ["maritime_sensors.h", "ondocean_mqtt.h"]:
        src_file = src_dir / header_file
        dst_file = dst_dir / header_file
        if src_file.exists():
            shutil.move(str(src_file), str(dst_file))
            print(f"Moved {header_file} to library")
    
    print(f"Arduino sketch structure created")
    return True

def validate_configuration():
    """Validate maritime configuration"""
    print(f"\nValidating configuration...")
    
    config_file = os.path.join(ONDOCEAN_REMOTEID_PATH, "maritime_config.json")
    if not os.path.exists(config_file):
        print(f"Configuration file not found: {config_file}")
        return False
    
    try:
        with open(config_file, 'r') as f:
            config = json.load(f)
        
        # Validate required sections
        required_sections = ["device", "gnss", "mqtt", "maritime"]
        for section in required_sections:
            if section not in config:
                print(f"Missing configuration section: {section}")
                return False
            else:
                print(f"Configuration section found: {section}")
        
        return True
        
    except json.JSONDecodeError as e:
        print(f"Invalid JSON in configuration: {e}")
        return False

def create_compile_script():
    """Create compilation script"""
    print(f"\nCreating compilation script...")
    
    compile_script = f"""@echo off
REM OndOcean RemoteID Compilation Script

echo Compiling OndOcean RemoteID for ESP32-S3...

"{ARDUINO_CLI_PATH}" compile ^
    --fqbn esp32:esp32:esp32s3:CDCOnBoot=cdc,CPUFreq=240,FlashFreq=80,FlashMode=qio,FlashSize=16M,LoopCore=1,EventsCore=1,USBMode=hwcdc,WDT=enable,PartitionScheme=app3M_fat9M_fact512k_16MB,PSRAM=enabled ^
    --output-dir build ^
    --verbose ^
    OndOceanRemoteID.ino

if %ERRORLEVEL% EQU 0 (
    echo Compilation successful!
    echo Binary location: build\\OndOceanRemoteID.ino.bin
) else (
    echo Compilation failed!
    exit /b 1
)
"""
    
    script_path = os.path.join(ONDOCEAN_REMOTEID_PATH, "compile.bat")
    with open(script_path, "w") as f:
        f.write(compile_script)
    
    print(f"Compilation script created: compile.bat")
    return True

def create_upload_script():
    """Create upload script"""
    print(f"\nCreating upload script...")
    
    upload_script = f"""@echo off
REM OndOcean RemoteID Upload Script

set /p COM_PORT="Enter COM port (e.g., COM3): "

echo Uploading to ESP32-S3 on %COM_PORT%...

"{ARDUINO_CLI_PATH}" upload ^
    --fqbn esp32:esp32:esp32s3 ^
    --port %COM_PORT% ^
    --input-dir build ^
    --verbose

if %ERRORLEVEL% EQU 0 (
    echo Upload successful!
    echo Opening serial monitor...
    "{ARDUINO_CLI_PATH}" monitor --port %COM_PORT% --config baudrate=115200
) else (
    echo Upload failed!
    exit /b 1
)
"""
    
    script_path = os.path.join(ONDOCEAN_REMOTEID_PATH, "upload.bat")
    with open(script_path, "w") as f:
        f.write(upload_script)
    
    print(f"Upload script created: upload.bat")
    return True

def main():
    """Main setup function"""
    print("OndOcean RemoteID Development Setup")
    print("=" * 50)
    
    # Change to project directory
    os.chdir(ONDOCEAN_REMOTEID_PATH)
    
    success = True
    
    # Check prerequisites
    if not check_arduino_cli():
        success = False
    
    # Setup Arduino environment
    if success and not setup_arduino_environment():
        success = False
    
    # Copy dependencies
    if success and not copy_remoteid_dependencies():
        success = False
    
    # Create sketch structure
    if success and not create_arduino_sketch_structure():
        success = False
    
    # Validate configuration
    if success and not validate_configuration():
        success = False
    
    # Create build scripts
    if success:
        create_compile_script()
        create_upload_script()
    
    print("\n" + "=" * 50)
    if success:
        print("Development environment setup complete!")
        print("\nNext steps:")
        print("1. Connect ESP32-S3 via USB")
        print("2. Run: compile.bat")
        print("3. Run: upload.bat")
        print("4. Monitor serial output")
    else:
        print("Setup failed. Please resolve errors above.")
    
    return success

if __name__ == "__main__":
    sys.exit(0 if main() else 1)
