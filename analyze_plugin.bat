@echo off
echo ==========================================
echo   Analizando custom-export-panel.dll
echo ==========================================
echo.
echo Abriendo Dependencies para analizar el plugin...
echo.
echo INSTRUCCIONES:
echo 1. Dependencies se abrira automaticamente
echo 2. Busca DLLs marcadas en ROJO (no encontradas)
echo 3. Busca DLLs marcadas en AMARILLO (version incorrecta)
echo 4. Anota cuales DLLs Qt estan causando problemas
echo.
echo Presiona cualquier tecla para abrir Dependencies...
pause >nul

start "" "Dependencies\DependenciesGui.exe" "%CD%\build\Release\custom-export-panel.dll"

echo.
echo Dependencies se ha abierto.
echo Revisa la ventana de Dependencies para ver los problemas.
echo.
pause
