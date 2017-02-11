@echo off
tasklist /FI "IMAGENAME eq NaMacroD.exe" 2>NUL | find /I /N "NaMacroD.exe">NUL
if "%ERRORLEVEL%" == "0" (
    echo # CheckProcess: NaMacroD is running... kill process.
    taskkill /f /im NaMacroD.exe
) else (
    echo # CheckProcess: NaMacroD is not running...
)
