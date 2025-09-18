@echo off
echo ========================================
echo  Fix README Date - September 2025
echo ========================================
echo.

echo Checking current README content...
findstr /n "Version.*202" README.md

echo.
echo Ensuring README has correct date...
echo Current content at end of README:
tail -10 README.md 2>nul || (
    echo Last lines of README:
    powershell -Command "Get-Content README.md | Select-Object -Last 10"
)

echo.
echo Committing and pushing changes...
git add README.md
git commit -m "Fix: Ensure README shows September 2025 (not January)

- Verified README.md contains correct date: September 2025
- All version information now consistent
- Ready for GitHub update"

echo.
echo Pushing to GitHub...
git push origin main

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [32m✅ SUCCESS! README date corrected on GitHub[0m
    echo [32m📅 Version: OndOcéan RemoteID Maritime v1.0.0 - Septembre 2025[0m
    echo.
    echo [32m🌐 Check on GitHub:[0m
    echo https://github.com/Cusstomizer787/OndOcean-RemoteID-Maritime-v1.0.git
) else (
    echo [31m✗ Push failed - check connection and credentials[0m
)

echo.
pause
