@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>nul
echo Checking Lib Exports...
dumpbin /LINKERMEMBER:1 "libs\obs-frontend-api.lib" | findstr "obs_frontend_get_profile_config"
echo.
echo Checking obs.lib Exports...
dumpbin /LINKERMEMBER:1 "libs\obs.lib" | findstr "config_get_string"
pause
