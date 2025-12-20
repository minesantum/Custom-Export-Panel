@echo off
setlocal

echo ==========================================
echo      Custom Export Panel Build V6
echo ==========================================

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
set "OBS_BIN_DIR=%DOWNLOADS_DIR%\obs-studio-windows-x64-407944a27\obs-studio-32.0.4-407944a27-windows-x64\bin\64bit"
set "QT_DEPS_ROOT=%DOWNLOADS_DIR%\windows-deps-qt6-2025-08-23-x64"
set "QT_DIR=%QT_DEPS_ROOT%\lib\cmake\Qt6"
set "OBS_SRC_DIR=%DOWNLOADS_DIR%\obs-studio"

REM Cleaning
if exist "build" rmdir /s /q build
if exist "libs" rmdir /s /q libs
mkdir libs

REM 3. GENERATE LIB FILES
echo [INFO] Parsing DLLs to create .def files...

powershell -ExecutionPolicy Bypass -File .\gen_libs.ps1 -dlldir "%OBS_BIN_DIR%" -outdir "%CD%\libs"

echo [INFO] Compiling .lib files...
cd libs
for %%f in (*.def) do (
    echo Generating lib for %%f...
    lib /DEF:%%f /OUT:%%~nf.lib /MACHINE:x64
    if errorlevel 1 echo [ERROR] Failed to generate lib for %%f
)
cd ..

REM 4. COMPILE
mkdir build
echo [INFO] Configuring CMake...

cmake -S . -B build ^
    -DQt6_DIR="%QT_DIR%" ^
    -DOBS_SOURCE_DIR="%OBS_SRC_DIR%" ^
    -DCMAKE_PREFIX_PATH="%QT_DEPS_ROOT%"

if %errorlevel% neq 0 (
    echo [ERROR] CMake Configuration Failed.
    pause
    exit /b 1
)

echo [INFO] Building...
cmake --build build --config Release

if %errorlevel% neq 0 (
    echo [ERROR] Build Failed.
    pause
    exit /b 1
)

echo.
echo [SUCCESS] Plugin compiled successfully!
echo Location: %CD%\build\Release\custom-export-panel.dll
pause
