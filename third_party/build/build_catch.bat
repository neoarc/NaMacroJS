@echo off
:: setlocal enabledelayedexpansion
setlocal
set thisScriptDir=%~dp0
pushd %thisScriptDir%

set libName=catch
set libVer=1.9.1
set incDirSrc=..\src\%libName%-%libVer%\include
set incDir=..\include\%libName%

:main
if exist %incDir% rmdir /q /s %incDir%
xcopy %incDirSrc% %incDir% /iey

:end
popd
endlocal
@echo on
@exit /b %ERRORLEVEL%
