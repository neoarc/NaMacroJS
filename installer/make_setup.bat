@echo off
setlocal

set thisScriptDir=%~dp0
:: save cur dir, and change cur dir as thisScriptDir
pushd %thisScriptDir%
set thisScriptFileName=%~nx0

:main

cd ..\build
call build.bat

cd ..\installer
call _gather_files.bat
call _compile_setup.bat

:end
popd
endlocal
@echo on
@exit /b %ERRORLEVEL%
