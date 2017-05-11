@echo off
setlocal

set setupFile=output\NaMacroJS-setup.exe
if not exist %setupFile% ( echo %setupFile% does not exist! & pause & exit /b %ERRORLEVEL%)

%setupFile% /SILENT /LOG
:: log in %LOCALAPPDATA%\Temp\Setup Log 2017-05-09 #001.txt

endlocal
@echo on
@exit /b %ERRORLEVEL%