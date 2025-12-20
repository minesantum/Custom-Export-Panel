@echo off
setlocal

set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
call "%VCVARS%" >nul 2>nul

set "DLL_PATH=C:\Users\DonKolia\Downloads\Custom Export Panel\build\Release\custom-export-panel.dll"
set "OUT_FILE=C:\Users\DonKolia\Downloads\Custom Export Panel\dll_info.txt"

echo === DLL INFO === > "%OUT_FILE%"
echo Build Time: %DATE% %TIME% >> "%OUT_FILE%"
echo Path: %DLL_PATH% >> "%OUT_FILE%"

echo. >> "%OUT_FILE%"
echo === DEPENDENCIES === >> "%OUT_FILE%"
dumpbin /DEPENDENTS "%DLL_PATH%" >> "%OUT_FILE%"

echo. >> "%OUT_FILE%"
echo === IMPORTS (obs.dll) === >> "%OUT_FILE%"
dumpbin /IMPORTS:obs.dll "%DLL_PATH%" >> "%OUT_FILE%"

echo. >> "%OUT_FILE%"
echo === IMPORTS (obs-frontend-api.dll) === >> "%OUT_FILE%"
dumpbin /IMPORTS:obs-frontend-api.dll "%DLL_PATH%" >> "%OUT_FILE%"

echo Done. Check dll_info.txt
type "%OUT_FILE%"
