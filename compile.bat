@echo off
setlocal

echo ==========================================
echo      Custom Export Panel Build Script
echo ==========================================

REM =========================================================
REM  PATHS DETECTED FROM USER INPUT
REM =========================================================
set "DOWNLOADS_DIR=C:\Users\DonKolia\Downloads"
set "OBS_DIR=C:\Users\DonKolia\Downloads\obs-studio"
set "QT_DEPS_DIR=%DOWNLOADS_DIR%\%DEPS_ARTIFACT_NAME%"

REM =========================================================
REM  QT PATH DETECTION (Updated for C:\Qt)
REM =========================================================

REM 1. Try to find a valid Qt 6 installation in C:\Qt
if exist "C:\Qt" (
    echo [INFO] Found C:\Qt, searching for Qt 6.x installation...
    
    REM Try specific version 6.8.3 first (as per README)
    if exist "C:\Qt\6.8.3\msvc2019_64\lib\cmake\Qt6" (
        set "Qt6_DIR=C:\Qt\6.8.3\msvc2019_64\lib\cmake\Qt6"
    ) else if exist "C:\Qt\6.8.3\msvc2022_64\lib\cmake\Qt6" (
        set "Qt6_DIR=C:\Qt\6.8.3\msvc2022_64\lib\cmake\Qt6"
    ) else (
        REM Try to find any 6.x version
        for /d %%v in (C:\Qt\6.*) do (
            for /d %%c in (%%v\msvc*) do (
                if exist "%%c\lib\cmake\Qt6" (
                    set "Qt6_DIR=%%c\lib\cmake\Qt6"
                )
            )
        )
    )
)

REM 2. Fallback to Downloads folder if not found in C:\Qt
if "%Qt6_DIR%"=="" (
    echo [INFO] Qt not auto-detected in C:\Qt, checking Downloads...
    set "Qt6_DIR=%QT_DEPS_DIR%\lib\cmake\Qt6"
)

REM OBS CMake config is usually in lib/cmake/libobs or cmake/libobs within the artifact
if exist "%OBS_DIR%\cmake\libobs\libobsConfig.cmake" (
    set "libobs_DIR=%OBS_DIR%\cmake\libobs"
) else (
    if exist "%OBS_DIR%\lib\cmake\libobs\libobsConfig.cmake" (
        set "libobs_DIR=%OBS_DIR%\lib\cmake\libobs"
    ) else (
        set "libobs_DIR=%OBS_DIR%"
    )
)

echo [INFO] directories set:
echo    - OBS: %OBS_DIR%
echo    - Qt6: %Qt6_DIR%
echo    - libobs CMake: %libobs_DIR%
echo.

REM Check if these exist
if not exist "%Qt6_DIR%" (
    echo [WARNING] Could not find Qt6 CMake directory at: %Qt6_DIR%
    echo Please verify the folder structure of your dependencies.
)

if not exist "%libobs_DIR%\libobsConfig.cmake" (
    echo [WARNING] Could not find libobsConfig.cmake in %libobs_DIR%
    echo Please verify the folder structure of your OBS binaries.
)

if not exist "build" mkdir build

echo [INFO] Configuring CMake...
REM We pass the specific DIR variables that find_package uses
cmake -S . -B build ^
    -DQt6_DIR="%Qt6_DIR%" ^
    -Dlibobs_DIR="%libobs_DIR%" ^
    -DOBS_SOURCE_DIR="%OBS_DIR%" ^
    -DCMAKE_PREFIX_PATH="%QT_DEPS_DIR%;%OBS_DIR%"

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] CMake Configuration failed.
    echo Common issues:
    echo  1. The path to Qt6Config.cmake is incorrect.
    echo  2. The path to libobsConfig.cmake is incorrect.
    echo  3. The downloaded OBS artifact is just binaries and lacks headers/CMake files.
    pause
    exit /b 1
)

echo.
echo [INFO] Building...
cmake --build build --config Release

if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    pause
    exit /b 1
)

echo.
echo [SUCCESS] Plugin compiled successfully!
echo The .dll file should be in build\Release\
pause
