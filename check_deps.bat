@echo off
setlocal

echo ==========================================
echo      Dependency Checker
echo ==========================================

REM 1. SETUP ENVIRONMENT
set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

if not exist "%VCVARS%" (
    echo [ERROR] Could not find vcvars64.bat
    exit /b 1
)

call "%VCVARS%" >nul 2>nul

echo [INFO] Checking dependencies for custom-export-panel.dll...
dumpbin /DEPENDENTS "C:\Users\DonKolia\Downloads\Custom Export Panel\build\Release\custom-export-panel.dll"

echo.
echo [INFO] Checking dependencies for Qt6Widgets.dll (reference)...
dumpbin /DEPENDENTS "C:\Users\DonKolia\Downloads\windows-deps-qt6-2025-08-23-x64\bin\Qt6Widgets.dll"

pause
