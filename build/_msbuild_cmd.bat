@echo off
:: setlocal enabledelayedexpansion
setlocal
pushd "%~dp0"

if "%1" == "" goto :usage
if "%2" == "" goto :usage

:main
set SLN=%1
set CONFIG=%2

set PLATFORM=Win32
set TOOLSET=v143

echo.
echo.
echo ########################################################################
echo ####
echo #### Starting Visual Studio Build
echo ####   - Solution: %SLN%
echo ####   - Config  : %CONFIG%
echo ####   - Platform: %PLATFORM%
echo ####   - Toolset : %TOOLSET%
echo ####
echo ########################################################################
echo.
echo.

::set VCVARSALL_BAT=%VS143COMNTOOLS:Common7\Tools\=VC\vcvarsall.bat%
@call _find_vcvarsall.bat
@call _vcvarsall.bat
@echo VCVARSALL_BAT = %VCVARSALL_BAT%
call "%VCVARSALL_BAT%" x86

set CFG=/property:Configuration=%CONFIG%
if not "%PLATFORM%" == "" (
    set CFG=%CFG%;Platform=%PLATFORM%
)

set CPU_CNT=/maxcpucount:4
set CMN_OPTS=%SLN% %CFG% %CPU_CNT%

MSBuild %CMN_OPTS% /target:Clean
if %ERRORLEVEL% neq 0 goto :end

MSBuild %CMN_OPTS% /target:Build
if %ERRORLEVEL% neq 0 goto :end

goto :end

:usage
echo Usage:
echo ------
echo msbuild_cmd.bat {solution_file_path} {config_name}
echo   ex) msbuild_cmd.bat my_project.sln Release

:end
popd
endlocal
@echo on
@exit /b %ERRORLEVEL%


