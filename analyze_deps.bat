@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>nul

echo ==========================================
echo   Analizando Dependencias del Plugin
echo ==========================================
echo.

echo [1] Dependencias directas:
dumpbin /DEPENDENTS "build\Release\custom-export-panel.dll"

echo.
echo.
echo [2] Verificando si las DLLs existen en el sistema:
echo.

set "OBS_BIN=C:\Users\DonKolia\Downloads\obs-studio-windows-x64-407944a27\obs-studio-32.0.4-407944a27-windows-x64\bin\64bit"

echo Buscando Qt6Widgets.dll...
if exist "%OBS_BIN%\Qt6Widgets.dll" (
    echo [OK] Qt6Widgets.dll encontrada en OBS
) else (
    echo [ERROR] Qt6Widgets.dll NO encontrada
)

echo Buscando Qt6Gui.dll...
if exist "%OBS_BIN%\Qt6Gui.dll" (
    echo [OK] Qt6Gui.dll encontrada en OBS
) else (
    echo [ERROR] Qt6Gui.dll NO encontrada
)

echo Buscando Qt6Core.dll...
if exist "%OBS_BIN%\Qt6Core.dll" (
    echo [OK] Qt6Core.dll encontrada en OBS
) else (
    echo [ERROR] Qt6Core.dll NO encontrada
)

echo Buscando obs.dll...
if exist "%OBS_BIN%\obs.dll" (
    echo [OK] obs.dll encontrada en OBS
) else (
    echo [ERROR] obs.dll NO encontrada
)

echo Buscando obs-frontend-api.dll...
if exist "%OBS_BIN%\obs-frontend-api.dll" (
    echo [OK] obs-frontend-api.dll encontrada en OBS
) else (
    echo [ERROR] obs-frontend-api.dll NO encontrada
)

echo.
echo [3] Verificando versiones de Qt:
echo.
echo Version de Qt en OBS:
dumpbin /HEADERS "%OBS_BIN%\Qt6Widgets.dll" | findstr /C:"image version"

echo.
pause
