@echo off
REM Simple compilation test for OndOcean RemoteID

echo Testing compilation without Arduino CLI...
echo.

REM Check if we have all required files
echo Checking required files:

if exist "OndOceanRemoteID.ino" (
    echo [OK] Main firmware file found
) else (
    echo [ERROR] OndOceanRemoteID.ino not found
    exit /b 1
)

if exist "board_config_maritime.h" (
    echo [OK] Maritime board config found
) else (
    echo [ERROR] board_config_maritime.h not found
    exit /b 1
)

if exist "maritime_sensors.h" (
    echo [OK] Maritime sensors header found
) else (
    echo [ERROR] maritime_sensors.h not found
    exit /b 1
)

if exist "maritime_sensors.cpp" (
    echo [OK] Maritime sensors implementation found
) else (
    echo [ERROR] maritime_sensors.cpp not found
    exit /b 1
)

if exist "ondocean_mqtt.h" (
    echo [OK] MQTT header found
) else (
    echo [ERROR] ondocean_mqtt.h not found
    exit /b 1
)

if exist "ondocean_mqtt.cpp" (
    echo [OK] MQTT implementation found
) else (
    echo [ERROR] ondocean_mqtt.cpp not found
    exit /b 1
)

if exist "maritime_config.json" (
    echo [OK] Configuration file found
) else (
    echo [ERROR] maritime_config.json not found
    exit /b 1
)

echo.
echo All required files present!
echo.
echo Next steps:
echo 1. Install Arduino CLI from: https://arduino.github.io/arduino-cli/
echo 2. Run setup_development.py again
echo 3. Use compile.bat to build firmware
echo 4. Use upload.bat to flash ESP32-S3
echo.
echo Hardware requirements:
echo - ESP32-S3 development board
echo - USB-C cable for programming
echo - u-blox F9P GNSS module (optional for testing)
echo - Maritime sensors (BME280, LSM6DS3, etc.)
echo.

pause
