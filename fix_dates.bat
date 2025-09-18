@echo off
echo ========================================
echo  OndOcean RemoteID Maritime
echo  Date Correction Script
echo ========================================
echo.

echo Checking and fixing all dates in project...
echo.

REM Check current dates
echo [33mCurrent version information:[0m
findstr /n "Version.*202" *.md 2>nul
findstr /n "Septembre\|septembre" *.md 2>nul
findstr /n "OndOcean202" *.ino *.cpp *.h 2>nul

echo.
echo [33mChecking for incorrect dates:[0m

REM Search for 2024 references
findstr /n "2024" *.ino *.cpp *.h *.md 2>nul | findstr /v "build\|\.map" >temp_2024.txt
if exist temp_2024.txt (
    echo [31m✗ Found 2024 references:[0m
    type temp_2024.txt
    del temp_2024.txt
) else (
    echo [32m✓ No 2024 references found[0m
)

REM Search for January references
findstr /n /i "janvier\|january" *.md *.h *.cpp *.ino 2>nul >temp_jan.txt
if exist temp_jan.txt (
    echo [31m✗ Found January references:[0m
    type temp_jan.txt
    del temp_jan.txt
) else (
    echo [32m✓ No January references found[0m
)

echo.
echo [33mVersion constants check:[0m
findstr /n "ONDOCEAN_VERSION" git-version.h 2>nul
findstr /n "VERSION_MAJOR\|VERSION_MINOR\|VERSION_PATCH" git-version.h 2>nul

echo.
echo [32m✓ Date verification complete![0m
echo.
echo Current correct version: OndOcéan RemoteID Maritime v1.0.0 - Septembre 2025
echo WiFi Password: OndOcean2025
echo.
echo ========================================
pause
