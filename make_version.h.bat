@echo off
setlocal ENABLEDELAYEDEXPANSION

set thisScriptDir=%~dp0
:: save cur dir, and change cur dir as thisScriptDir
pushd %thisScriptDir%
set thisScriptFileName=%~nx0

:: http://semver.org/
set MAJOR_VERSION=0
set MINOR_VERSION=5
set PATCH_VERSION=0

set COMPANY_NAME=NeoArc Solution
set PRODUCT_NAME=NaMacroJS

for /F "tokens=* USEBACKQ" %%F in (`git rev-list --count --first-parent HEAD`) do (
  set REVISION_NUMBER=%%F
)

set FILE_VERSION=%MAJOR_VERSION%,%MINOR_VERSION%,%PATCH_VERSION%,%REVISION_NUMBER%
set FILE_VERSION_STR=%MAJOR_VERSION%.%MINOR_VERSION%.%PATCH_VERSION%.%REVISION_NUMBER%

@set YEAR=%date:~0,4%
@set MONTH=%date:~5,2%
@set DAY=%date:~8,2%
@set HOUR=%time:~0,2%
@if "%HOUR:~0,1%" == " " set HOUR=0%HOUR:~1,1%
@set MINUTE=%time:~3,2%
@if "%MINUTE:~0,1%" == " " set MINUTE=0%MINUTE:~1,1%
@set SECOND=%time:~6,2%
@if "%SECOND:~0,1%" == " " set SECOND=0%SECOND:~1,1%

@set BUILD_DATETIME=%YEAR%.%MONTH%.%DAY%_%HOUR%:%MINUTE%:%SECOND%
echo * Build Datetime = %BUILD_DATETIME%

(
    echo #define MAJOR_VERSION           %MAJOR_VERSION%
    echo #define MAJOR_VERSION_STR      "%MAJOR_VERSION%"
    echo #define MINOR_VERSION           %MINOR_VERSION%
    echo #define MINOR_VERSION_STR      "%MINOR_VERSION%"
    echo #define PATCH_VERSION           %PATCH_VERSION%
    echo #define PATCH_VERSION_STR      "%PATCH_VERSION%"
    echo #define REVISION_NUMBER         %REVISION_NUMBER%
    echo #define REVISION_NUMBER_STR    "%REVISION_NUMBER%"
    echo.
    echo #define FILE_VERSION            %FILE_VERSION%
    echo #define FILE_VERSION_STR       "%FILE_VERSION_STR%"
    echo #define PRODUCT_VERSION         FILE_VERSION
    echo #define PRODUCT_VERSION_STR     FILE_VERSION_STR
    echo.
    echo #define COMPANY_NAME           "%COMPANY_NAME%"
    echo #define PRODUCT_NAME           "%PRODUCT_NAME%"
    echo #define PRODUCT_URL            "https://github.com/neoarc/NaMacroJS"
    echo.
    echo #define BUILD_DATETIME_STR     "%BUILD_DATETIME%"
) > version.h