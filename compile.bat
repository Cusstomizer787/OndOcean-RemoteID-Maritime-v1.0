@echo off
REM OndOcean RemoteID Compilation Script

echo ========================================
echo  OndOcean RemoteID - ESP32-S3 Compile
echo ========================================

echo.
echo Compiling OndOcean RemoteID for ESP32-S3...

REM Create build directory
if not exist "build" mkdir build

echo.
echo Using Arduino CLI: "C:\Program Files\Arduino CLI\arduino-cli.exe"

"C:\Program Files\Arduino CLI\arduino-cli.exe" compile ^
    --fqbn esp32:esp32:esp32s3:PSRAM=enabled ^
    --output-dir build ^
    --verbose ^
    ondocean-remoteid.ino

echo.
if %ERRORLEVEL% EQU 0 (
    echo ========================================
    echo  COMPILATION SUCCESSFUL!
    echo ========================================
    echo.
    echo Binary files created:
    echo - build\ondocean-remoteid.ino.bin
    echo - build\ondocean-remoteid.ino.elf
    echo.
    echo Next steps:
    echo 1. Connect ESP32-S3 via USB-C
    echo 2. Run upload.bat to flash firmware
    echo 3. Use serial monitor to view output
    echo.
) else (
    echo ========================================
    echo  COMPILATION FAILED!
    echo ========================================
    echo.
    echo Check the error messages above.
    echo Common issues:
    echo - Missing libraries (run install_esp32_support.bat)
    echo - Syntax errors in code
    echo - Missing include files
    echo.
    exit /b 1
)

pause
