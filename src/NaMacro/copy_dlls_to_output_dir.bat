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
call :copy_a_dll icui18n.dll
call :copy_a_dll icuuc.dll
call :copy_a_dll v8.dll
call :copy_a_dll v8_libbase.dll
call :copy_a_dll v8_libplatform.dll
::copy ..\..\packages\v8.redist-v140-x86.5.6.326.55\lib\%Configuration%\*.dll ..\..\output\%Configuration%

goto :end

:copy_a_dll
set dllName=%1
set dllSrcPath=..\..\packages\v8.redist-v140-x86.5.6.326.55\lib\%Configuration%\%dllName%
set dllDstPath=..\..\output\%Configuration%\%dllName%
if not exist %dllDstPath% (
  echo %dllName%
  copy %dllSrcPath% %dllDstPath%
)
@exit /b %ERRORLEVEL%

:usage
echo Usage: %thisScriptFileName% ^<Configuration^>
echo ------
echo   ex) %thisScriptFileName% Release

:end
popd
endlocal
@echo on
@exit /b %ERRORLEVEL%
