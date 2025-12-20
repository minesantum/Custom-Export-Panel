@echo off
echo ==========================================
echo   Instalando Plugin SIMPLE (Sin Qt)
echo ==========================================
echo.

set "OBS_BIN=C:\Users\DonKolia\Downloads\obs-studio-windows-x64-407944a27\obs-studio-32.0.4-407944a27-windows-x64\bin\64bit"
set "OBS_PLUGINS=%OBS_BIN%\..\..\obs-plugins\64bit"
set "PLUGIN_DLL=build\Release\custom-export-panel.dll"
set "CONFIG_DIR=%APPDATA%\obs-studio\plugin_config\custom-export-panel"

echo [1] Copiando plugin...
copy /Y "%PLUGIN_DLL%" "%OBS_PLUGINS%\"
if %errorlevel% neq 0 (
    echo [ERROR] No se pudo copiar el plugin
    pause
    exit /b 1
)
echo [OK] Plugin copiado

echo.
echo [2] Copiando dependencias OBS...
copy /Y "%OBS_BIN%\obs.dll" "%OBS_PLUGINS%\"
copy /Y "%OBS_BIN%\obs-frontend-api.dll" "%OBS_PLUGINS%\"
echo [OK] DLLs de OBS copiadas

echo.
echo [3] Creando directorio de configuración...
if not exist "%CONFIG_DIR%" mkdir "%CONFIG_DIR%"
echo [OK] Directorio creado: %CONFIG_DIR%

echo.
echo [4] Copiando archivo de configuración de ejemplo...
copy /Y "export_config.ini.example" "%CONFIG_DIR%\export_config.ini.example"
echo [OK] Ejemplo copiado

echo.
echo ==========================================
echo   INSTALACION COMPLETADA
echo ==========================================
echo.
echo El plugin ha sido instalado en:
echo %OBS_PLUGINS%\custom-export-panel.dll
echo.
echo CONFIGURACION:
echo 1. Edita el archivo: %CONFIG_DIR%\export_config.ini
echo 2. Configura tu ruta y nombre de archivo preferidos
echo 3. Reinicia OBS
echo.
echo NOTA: Esta version NO tiene interfaz grafica.
echo      Usa el archivo INI para configurar las rutas.
echo.
pause
