@echo off
setlocal

echo ==========================================
echo   Custom Export Panel - Compilacion FINAL
echo   Con Qt 6.8.3 y UI Completa
echo ==========================================
echo.

REM 1. SETUP ENVIRONMENT
echo [INFO] Setting up VS Environment...
set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

if not exist "%VCVARS%" (
    for /f "delims=" %%i in ('dir /s /b "C:\Program Files\Microsoft Visual Studio\2022\vcvars64.bat"') do set "VCVARS=%%i"
)

call "%VCVARS%" >nul 2>nul
echo [INFO] VS Environment ready.

REM 2. DEFINE PATHS
set "DOWNLOADS_DIR=C:\Users\DonKolia\Downloads"
set "OBS_SRC_DIR=%DOWNLOADS_DIR%\obs-studio"
set "QT_DIR=C:\Qt\6.8.3\msvc2022_64"
set "CMAKE_PREFIX_PATH=%QT_DIR%"

echo [INFO] Qt 6.8.3 Path: %QT_DIR%

REM 3. VERIFICAR QUE LIBS EXISTEN
if not exist "libs\obs.lib" (
    echo [ERROR] libs\obs.lib no encontrado. Ejecuta compile_v2.bat primero.
    pause
    exit /b 1
)

REM 4. CLEAN BUILD
if exist "build" rmdir /s /q build
mkdir build

REM 5. CONFIGURE CMAKE
echo [INFO] Configuring CMake with Qt 6.8.3...
cmake -S . -B build ^
    -DQt6_DIR="%QT_DIR%\lib\cmake\Qt6" ^
    -DOBS_SOURCE_DIR="%OBS_SRC_DIR%" ^
    -DCMAKE_PREFIX_PATH="%QT_DIR%"

if %errorlevel% neq 0 (
    echo [ERROR] CMake Configuration Failed.
    pause
    exit /b 1
)

REM 6. BUILD
echo [INFO] Building plugin with UI...
cmake --build build --config Release

if %errorlevel% neq 0 (
    echo [ERROR] Build Failed.
    pause
    exit /b 1
)

echo.
echo ==========================================
echo   COMPILACION EXITOSA!
echo ==========================================
echo.
echo Plugin compilado: build\Release\custom-export-panel.dll
echo.
echo Ahora ejecuta: install_plugin_ui.bat
echo.
pause
