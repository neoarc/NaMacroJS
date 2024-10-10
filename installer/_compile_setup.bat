@echo off
setlocal

set thisScriptDir=%~dp0
:: save cur dir, and change cur dir as thisScriptDir
pushd %thisScriptDir%
set thisScriptFileName=%~nx0

:main
call ..\make_version.h.bat
set innoSetupCompiler=c:\Program Files (x86)\Inno Setup 6\ISCC.exe
"%innoSetupCompiler%" _NaMacroJS-setup.iss

:end
popd
endlocal
@echo on
@exit /b %ERRORLEVEL%