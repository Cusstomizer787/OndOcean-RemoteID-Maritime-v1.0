@echo off
echo Installing ESP32 support for Arduino CLI...

echo.
echo Step 1: Updating core index...
"C:\Program Files\Arduino CLI\arduino-cli.exe" core update-index

echo.
echo Step 2: Installing ESP32 core (this may take several minutes)...
"C:\Program Files\Arduino CLI\arduino-cli.exe" core install esp32:esp32

echo.
echo Step 3: Installing required libraries...
"C:\Program Files\Arduino CLI\arduino-cli.exe" lib install ArduinoJson
"C:\Program Files\Arduino CLI\arduino-cli.exe" lib install PubSubClient

echo.
echo Step 4: Verifying installation...
echo Installed cores:
"C:\Program Files\Arduino CLI\arduino-cli.exe" core list

echo.
echo Installed libraries:
"C:\Program Files\Arduino CLI\arduino-cli.exe" lib list

echo.
echo ESP32 support installation complete!
echo You can now compile the OndOcean RemoteID firmware.
pause
