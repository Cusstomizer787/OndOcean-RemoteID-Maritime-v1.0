@echo off
echo ========================================
echo  OndOcean RemoteID Maritime
echo  GitHub Update Script
echo ========================================
echo.

echo [33mStep 1: Checking Git status...[0m
git status

echo.
echo [33mStep 2: Adding all changes...[0m
git add .

echo.
echo [33mStep 3: Checking what will be committed...[0m
git status --short

echo.
echo [33mStep 4: Committing changes...[0m
git commit -m "Fix: Update all dates to September 2025 and improve version management

✅ Corrections Applied:
- Updated WiFi password from OndOcean2024 to OndOcean2025
- Added comprehensive version constants in git-version.h
- Added version display in firmware startup
- Created date verification script (fix_dates.bat)
- Ensured all dates are consistent: September 2025

🎯 Version: OndOcéan RemoteID Maritime v1.0.0 - Septembre 2025
📅 All dates now consistent and correct
🔧 Ready for maritime deployment"

if %ERRORLEVEL% NEQ 0 (
    echo [31m✗ Commit failed![0m
    echo Check if there are changes to commit or fix any errors above.
    pause
    exit /b 1
)

echo.
echo [33mStep 5: Pushing to GitHub...[0m
git push origin main

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [32m✅ SUCCESS! Repository updated on GitHub[0m
    echo.
    echo [32m🌐 Repository URL:[0m
    echo https://github.com/Cusstomizer787/OndOcean-RemoteID-Maritime-v1.0.git
    echo.
    echo [32m📋 Changes pushed:[0m
    echo ✅ Date corrections (September 2025)
    echo ✅ Version management improvements  
    echo ✅ WiFi password updated
    echo ✅ Documentation consistency
    echo.
    echo [32m🎯 Current version: OndOcéan RemoteID Maritime v1.0.0 - Septembre 2025[0m
) else (
    echo.
    echo [31m✗ Push failed![0m
    echo.
    echo [33mPossible solutions:[0m
    echo 1. Check internet connection
    echo 2. Verify GitHub credentials
    echo 3. Check if repository exists and you have write access
    echo 4. Try: git pull origin main (if there are conflicts)
    echo.
    echo [33mManual commands to try:[0m
    echo git pull origin main
    echo git push origin main
)

echo.
echo ========================================
pause
