@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>nul
dumpbin /SYMBOLS "build\custom-export-panel.dir\Release\plugin-main.obj" | findstr "obs_frontend_get_profile_config"
echo.
echo Checking for config_get_string...
dumpbin /SYMBOLS "build\custom-export-panel.dir\Release\plugin-main.obj" | findstr "config_get_string"
pause
