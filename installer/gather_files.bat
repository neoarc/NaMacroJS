@echo off
setlocal

set thisScriptDir=%~dp0
:: save cur dir, and change cur dir as thisScriptDir
pushd %thisScriptDir%
set thisScriptFileName=%~nx0

:main

set binSrcDir=bin_src

call :prepare_bin_src_dir
call :gather_output_files
call :gather_v8_dlls
call :gather_vc_redist_dlls
:: call :gather_addon_scripts

goto :end

:prepare_bin_src_dir
echo . & echo %0:

if exist %binSrcDir% rmdir /q /s %binSrcDir%
if %ERRORLEVEL% neq 0 ( pause )
mkdir %binSrcDir%
if %ERRORLEVEL% neq 0 ( pause )
@exit /b %ERRORLEVEL%

:gather_output_files
echo . & echo %0: to %binSrcDir%

set outputDir=..\output\Release
call :copy_file %outputDir%\NaMacro.exe         %binSrcDir%
call :copy_file %outputDir%\NaMacroRecorder.exe %binSrcDir%
@exit /b %ERRORLEVEL%

:gather_v8_dlls
echo . & echo %0:

set v8BinDir=..\packages\v8.redist-v140-x86.5.6.326.55\lib\Release
call :copy_file %v8BinDir%\icui18n.dll          %binSrcDir%
call :copy_file %v8BinDir%\icuuc.dll            %binSrcDir%
call :copy_file %v8BinDir%\v8.dll               %binSrcDir%
call :copy_file %v8BinDir%\v8_libbase.dll       %binSrcDir%
call :copy_file %v8BinDir%\v8_libplatform.dll   %binSrcDir%
@exit /b %ERRORLEVEL%

:gather_vc_redist_dlls
echo . & echo %0:

set vc140root=%VS140COMNTOOLS%\..\..
set vcRedistCRT=%vc140root%\VC\redist\x86\Microsoft.VC140.CRT
call :copy_file "%vcRedistCRT%\msvcp140.dll"        %binSrcDir%
call :copy_file "%vcRedistCRT%\vcruntime140.dll"    %binSrcDir%

set vcRedistMFC=%vc140root%\VC\redist\x86\Microsoft.VC140.MFC
call :copy_file "%vcRedistMFC%\mfc140u.dll"         %binSrcDir%
@exit /b %ERRORLEVEL%

:copy_file
set srcPath=%1
set dstDir=%2
set copyCmd=copy %srcPath% %dstDir%
echo   %srcPath%
:: echo   %copyCmd%
%copyCmd%
if %ERRORLEVEL% neq 0 ( pause )
@exit /b %ERRORLEVEL%

:end
popd
endlocal
@echo on
@exit /b %ERRORLEVEL%