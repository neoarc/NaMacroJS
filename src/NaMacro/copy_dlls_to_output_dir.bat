@echo off
setlocal

set thisScriptDir=%~dp0
:: save cur dir, and change cur dir as thisScriptDir
pushd %thisScriptDir%
set thisScriptFileName=%~nx0

set Configuration=%1
if "%Configuration%" == "" goto :usage

if "%Configuration%" neq "Debug" if "%Configuration%" neq "Release" goto :usage

:main
copy ..\..\packages\v8.redist-v140-x86.5.6.326.55\lib\%Configuration%\*.dll ..\..\output\%Configuration%
goto :end

:usage
echo Usage: %thisScriptFileName% ^<Configuration^>
echo ------
echo   ex) %thisScriptFileName% Release

:end
popd
endlocal
@echo on
@exit /b %ERRORLEVEL%
