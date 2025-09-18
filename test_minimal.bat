@echo off
echo Testing minimal compilation...

"C:\Program Files\Arduino CLI\arduino-cli.exe" compile ^
    --fqbn esp32:esp32:esp32s3:PSRAM=enabled ^
    --output-dir build ^
    --verify ^
    ondocean-remoteid.ino

echo Compilation test complete.
pause
