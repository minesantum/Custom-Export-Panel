@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>nul
echo Testing Lib Generation...
lib /DEF:libs\Qt6Widgets.def /OUT:libs\Qt6Widgets.lib /MACHINE:x64
echo Exit Code: %errorlevel%
pause
