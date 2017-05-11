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
call gather_files.bat
call build_NaMacroJS-setup.bat

:end
popd
endlocal
@echo on
@exit /b %ERRORLEVEL%