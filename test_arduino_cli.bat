@echo off
echo Testing Arduino CLI installation...

echo.
echo 1. Checking Arduino CLI version:
"C:\Program Files\Arduino CLI\arduino-cli.exe" version

echo.
echo 2. Checking installed cores:
"C:\Program Files\Arduino CLI\arduino-cli.exe" core list

echo.
echo 3. Checking installed libraries:
"C:\Program Files\Arduino CLI\arduino-cli.exe" lib list

echo.
echo Test complete!
pause
