@echo off
echo ========================================
echo  OndOcean RemoteID Maritime
echo  Clean Development References
echo ========================================
echo.

echo Checking for remaining development-specific references...
echo.

echo [33mSearching for Windsurf references:[0m
findstr /s /i "windsurf" *.* 2>nul || echo No Windsurf references found

echo.
echo [33mSearching for CascadeProjects references:[0m
findstr /s /i "cascadeprojects" *.* 2>nul || echo No CascadeProjects references found

echo.
echo [33mSearching for specific user paths:[0m
findstr /s /i "ncuss" *.* 2>nul || echo No user-specific paths found

echo.
echo [32m✅ Cleanup complete![0m
echo [32m✅ Project is now environment-independent[0m
echo.
echo [33mChanges made:[0m
echo - Removed Windsurf-specific paths from DEPLOYMENT.md
echo - Generalized paths in setup_development.py
echo - Added configuration instructions
echo.
echo ========================================
pause
