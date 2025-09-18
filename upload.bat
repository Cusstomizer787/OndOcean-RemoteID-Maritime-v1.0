@echo off
REM OndOcean RemoteID Upload Script

echo ========================================
echo  OndOcean RemoteID - ESP32-S3 Upload
echo ========================================

REM Check if build directory exists
if not exist "build" (
    echo ERROR: Build directory not found!
    echo Please run compile.bat first.
    pause
    exit /b 1
)

REM Check if binary exists
if not exist "build\ondocean-remoteid.ino.bin" (
    echo ERROR: Compiled binary not found!
    echo Please run compile.bat first.
    pause
    exit /b 1
)

echo.
echo Available COM ports:
"C:\Program Files\Arduino CLI\arduino-cli.exe" board list

echo.
set /p COM_PORT="Enter COM port for ESP32-S3 (e.g., COM3): "

if "%COM_PORT%"=="" (
    echo ERROR: No COM port specified!
    pause
    exit /b 1
)

echo.
echo Uploading to ESP32-S3 on %COM_PORT%...
echo This may take 30-60 seconds...

"C:\Program Files\Arduino CLI\arduino-cli.exe" upload ^
    --fqbn esp32:esp32:esp32s3 ^
    --port %COM_PORT% ^
    --input-dir build ^
    --verbose

echo.
if %ERRORLEVEL% EQU 0 (
    echo ========================================
    echo  UPLOAD SUCCESSFUL!
    echo ========================================
    echo.
    echo The OndOcean RemoteID firmware is now running on ESP32-S3.
    echo.
    set /p MONITOR="Open serial monitor? (y/n): "
    if /i "%MONITOR%"=="y" (
        echo.
        echo Opening serial monitor (115200 baud)...
        echo Press Ctrl+C to exit monitor.
        echo.
        "C:\Program Files\Arduino CLI\arduino-cli.exe" monitor --port %COM_PORT% --config baudrate=115200
    )
) else (
    echo ========================================
    echo  UPLOAD FAILED!
    echo ========================================
    echo.
    echo Common issues:
    echo - Wrong COM port selected
    echo - ESP32-S3 not in download mode
    echo - USB cable not connected
    echo - Driver issues
    echo.
    echo Try:
    echo 1. Hold BOOT button while connecting USB
    echo 2. Check Device Manager for correct COM port
    echo 3. Try different USB cable/port
    echo.
    exit /b 1
)

pause
