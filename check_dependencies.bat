@echo off
echo ========================================
echo  OndOcean RemoteID Maritime
echo  Dependency Checker
echo ========================================
echo.

set ERROR_COUNT=0

REM Check Arduino CLI
echo Checking Arduino CLI...
arduino-cli version >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [32m✓ Arduino CLI found[0m
    arduino-cli version | findstr "arduino-cli"
) else (
    echo [31m✗ Arduino CLI not found[0m
    echo   Please install Arduino CLI from: https://arduino.github.io/arduino-cli/
    set /a ERROR_COUNT+=1
)
echo.

REM Check ESP32 Core
echo Checking ESP32 Core...
arduino-cli core list | findstr "esp32:esp32" >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [32m✓ ESP32 core found[0m
    arduino-cli core list | findstr "esp32:esp32"
) else (
    echo [31m✗ ESP32 core not found[0m
    echo   Install with: arduino-cli core install esp32:esp32@3.3.0
    set /a ERROR_COUNT+=1
)
echo.

REM Check Required Libraries
echo Checking required libraries...

REM ArduinoJson
arduino-cli lib list | findstr "ArduinoJson" >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [32m✓ ArduinoJson library found[0m
) else (
    echo [31m✗ ArduinoJson library missing[0m
    echo   Install with: arduino-cli lib install "ArduinoJson@7.4.2"
    set /a ERROR_COUNT+=1
)

REM PubSubClient
arduino-cli lib list | findstr "PubSubClient" >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [32m✓ PubSubClient library found[0m
) else (
    echo [31m✗ PubSubClient library missing[0m
    echo   Install with: arduino-cli lib install "PubSubClient@2.8"
    set /a ERROR_COUNT+=1
)

REM Adafruit NeoPixel
arduino-cli lib list | findstr "Adafruit NeoPixel" >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [32m✓ Adafruit NeoPixel library found[0m
) else (
    echo [31m✗ Adafruit NeoPixel library missing[0m
    echo   Install with: arduino-cli lib install "Adafruit NeoPixel@1.15.1"
    set /a ERROR_COUNT+=1
)
echo.

REM Check Project Files
echo Checking project files...
if exist "ondocean-remoteid.ino" (
    echo [32m✓ Main firmware file found[0m
) else (
    echo [31m✗ Main firmware file missing[0m
    set /a ERROR_COUNT+=1
)

if exist "board_config_maritime.h" (
    echo [32m✓ Maritime configuration found[0m
) else (
    echo [31m✗ Maritime configuration missing[0m
    set /a ERROR_COUNT+=1
)

if exist "ondocean_logger.h" (
    echo [32m✓ Logging system found[0m
) else (
    echo [31m✗ Logging system missing[0m
    set /a ERROR_COUNT+=1
)

if exist "data_validation.h" (
    echo [32m✓ Data validation system found[0m
) else (
    echo [31m✗ Data validation system missing[0m
    set /a ERROR_COUNT+=1
)
echo.

REM Summary
echo ========================================
if %ERROR_COUNT% EQU 0 (
    echo [32m✓ All dependencies satisfied![0m
    echo [32m✓ Ready for compilation[0m
    echo.
    echo Run: test_minimal.bat to test compilation
) else (
    echo [31m✗ %ERROR_COUNT% dependency issues found[0m
    echo [31m✗ Please resolve issues before compilation[0m
)
echo ========================================

pause
exit /b %ERROR_COUNT%
