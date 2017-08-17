tasklist /FI "IMAGENAME eq NaMacro.exe" | find /I /N "NaMacro.exe"
echo %ERRORLEVEL%
tasklist /FI "IMAGENAME eq NaMacroD.exe" | find /I /N "NaMacroD.exe"
echo %ERRORLEVEL%
pause