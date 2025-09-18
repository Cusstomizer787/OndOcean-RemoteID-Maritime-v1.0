@echo off
echo ========================================
echo  OndOcean RemoteID Maritime
echo  Pre-Push Verification
echo ========================================
echo.

set ERROR_COUNT=0

echo [33m1. Checking compilation...[0m
call test_minimal.bat >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [32m✓ Compilation successful[0m
) else (
    echo [31m✗ Compilation failed[0m
    set /a ERROR_COUNT+=1
)

echo.
echo [33m2. Checking dates consistency...[0m
call fix_dates.bat >temp_dates.log 2>&1
findstr /c "No 2024 references found" temp_dates.log >nul
if %ERRORLEVEL% EQU 0 (
    echo [32m✓ No incorrect 2024 references[0m
) else (
    echo [31m✗ Found 2024 references[0m
    set /a ERROR_COUNT+=1
)

findstr /c "No January references found" temp_dates.log >nul
if %ERRORLEVEL% EQU 0 (
    echo [32m✓ No January references[0m
) else (
    echo [31m✗ Found January references[0m
    set /a ERROR_COUNT+=1
)
del temp_dates.log >nul 2>&1

echo.
echo [33m3. Checking version consistency...[0m
findstr "Septembre 2025" README.md >nul
if %ERRORLEVEL% EQU 0 (
    echo [32m✓ README.md has correct date[0m
) else (
    echo [31m✗ README.md date incorrect[0m
    set /a ERROR_COUNT+=1
)

findstr "OndOcean2025" ondocean-remoteid.ino >nul
if %ERRORLEVEL% EQU 0 (
    echo [32m✓ WiFi password updated[0m
) else (
    echo [31m✗ WiFi password not updated[0m
    set /a ERROR_COUNT+=1
)

echo.
echo [33m4. Checking Git status...[0m
git status --porcelain >temp_git.txt
if exist temp_git.txt (
    for /f %%i in ('type temp_git.txt ^| find /c /v ""') do set FILE_COUNT=%%i
    if !FILE_COUNT! GTR 0 (
        echo [33m! %FILE_COUNT% files have changes to commit[0m
        echo [33mModified files:[0m
        type temp_git.txt
    ) else (
        echo [32m✓ No pending changes[0m
    )
    del temp_git.txt
)

echo.
echo ========================================
if %ERROR_COUNT% EQU 0 (
    echo [32m✅ ALL CHECKS PASSED![0m
    echo [32m✅ Ready to push to GitHub[0m
    echo.
    echo Run: update_github.bat
) else (
    echo [31m✗ %ERROR_COUNT% issues found[0m
    echo [31m✗ Please fix issues before pushing[0m
)
echo ========================================
pause
