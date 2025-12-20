@echo off
echo ==========================================
echo   Limpiando Versiones Antiguas
echo ==========================================
echo.

set "OBS_PLUGINS=C:\Users\DonKolia\Downloads\obs-studio-windows-x64-407944a27\obs-studio-32.0.4-407944a27-windows-x64\obs-plugins\64bit"

echo [1] Eliminando plugin antiguo...
del /F /Q "%OBS_PLUGINS%\custom-export-panel.dll" 2>nul
echo [OK] Plugin eliminado

echo.
echo [2] Eliminando DLLs antiguas de Qt...
del /F /Q "%OBS_PLUGINS%\Qt6Core.dll" 2>nul
del /F /Q "%OBS_PLUGINS%\Qt6Gui.dll" 2>nul
del /F /Q "%OBS_PLUGINS%\Qt6Widgets.dll" 2>nul
del /F /Q "%OBS_PLUGINS%\obs.dll" 2>nul
del /F /Q "%OBS_PLUGINS%\obs-frontend-api.dll" 2>nul
echo [OK] DLLs eliminadas

echo.
echo [3] Instalando version NUEVA con UI...
copy /Y "build\Release\custom-export-panel.dll" "%OBS_PLUGINS%\"
if %errorlevel% neq 0 (
    echo [ERROR] No se pudo copiar el plugin
    pause
    exit /b 1
)
echo [OK] Plugin nuevo instalado

echo.
echo [4] Copiando DLLs de Qt 6.8.3...
copy /Y "C:\Qt\6.8.3\msvc2022_64\bin\Qt6Core.dll" "%OBS_PLUGINS%\"
copy /Y "C:\Qt\6.8.3\msvc2022_64\bin\Qt6Gui.dll" "%OBS_PLUGINS%\"
copy /Y "C:\Qt\6.8.3\msvc2022_64\bin\Qt6Widgets.dll" "%OBS_PLUGINS%\"
echo [OK] DLLs de Qt copiadas

echo.
echo [5] Copiando dependencias OBS...
set "OBS_BIN=C:\Users\DonKolia\Downloads\obs-studio-windows-x64-407944a27\obs-studio-32.0.4-407944a27-windows-x64\bin\64bit"
copy /Y "%OBS_BIN%\obs.dll" "%OBS_PLUGINS%\"
copy /Y "%OBS_BIN%\obs-frontend-api.dll" "%OBS_PLUGINS%\"
echo [OK] DLLs de OBS copiadas

echo.
echo ==========================================
echo   INSTALACION LIMPIA COMPLETADA
echo ==========================================
echo.
echo REINICIA OBS y verifica el log.
echo Deberias ver los mensajes de depuracion.
echo.
pause
