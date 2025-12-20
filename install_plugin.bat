@echo off
echo ==========================================
echo   Instalando Plugin en OBS
echo ==========================================
echo.

set "OBS_BIN=C:\Users\DonKolia\Downloads\obs-studio-windows-x64-407944a27\obs-studio-32.0.4-407944a27-windows-x64\bin\64bit"
set "OBS_PLUGINS=%OBS_BIN%\..\..\obs-plugins\64bit"
set "PLUGIN_DLL=build\Release\custom-export-panel.dll"

echo [1] Copiando plugin...
copy /Y "%PLUGIN_DLL%" "%OBS_PLUGINS%\"
if %errorlevel% neq 0 (
    echo [ERROR] No se pudo copiar el plugin
    pause
    exit /b 1
)
echo [OK] Plugin copiado

echo.
echo [2] Copiando dependencias Qt necesarias...
copy /Y "%OBS_BIN%\Qt6Core.dll" "%OBS_PLUGINS%\"
copy /Y "%OBS_BIN%\Qt6Gui.dll" "%OBS_PLUGINS%\"
copy /Y "%OBS_BIN%\Qt6Widgets.dll" "%OBS_PLUGINS%\"
echo [OK] DLLs de Qt copiadas

echo.
echo [3] Copiando dependencias OBS...
copy /Y "%OBS_BIN%\obs.dll" "%OBS_PLUGINS%\"
copy /Y "%OBS_BIN%\obs-frontend-api.dll" "%OBS_PLUGINS%\"
echo [OK] DLLs de OBS copiadas

echo.
echo ==========================================
echo   INSTALACION COMPLETADA
echo ==========================================
echo.
echo El plugin ha sido instalado en:
echo %OBS_PLUGINS%\custom-export-panel.dll
echo.
echo IMPORTANTE: Reinicia OBS para cargar el plugin.
echo.
pause
