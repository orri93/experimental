@ECHO OFF

SETLOCAL ENABLEEXTENSIONS

SET PROJECT_ROOT_DIR=..\..
CALL:MakeAbsolute PROJECT_ROOT_DIR "%~dp0"

SET PROJECT_NAME=GOS WebAssembly D3 Heatmap Charting Experiment

SET EXE_CMAKE=cmake.exe
SET EXE_DOCKER=docker.exe

ECHO ---------------------------------------------------------------------------
ECHO Docker test script for the %PROJECT_NAME%
ECHO %PROJECT_NAME% root directory is defined as %PROJECT_ROOT_DIR%

SET WORKIN_DIR=%PROJECT_ROOT_DIR%\results\docker\tests

SET CONTAINER=wd3
SET REGISTRY=
REM FOR /f "delims=" %%x IN ("%PROJECT_ROOT_DIR%\version.txt") DO SET VERSION=%%x
SET /p VERSION=<"%PROJECT_ROOT_DIR%\version.txt"
REM SET IMAGE=%REGISTRY%/%CONTAINER%:%VERSION%-test
SET IMAGE=%CONTAINER%:%VERSION%-test
SET FILE=%PROJECT_ROOT_DIR%\docker\Dockerfile.test

ECHO - Work directory is defined as %WORKIN_DIR%
ECHO - Version is defined as %VERSION%
ECHO - Build number is defined as %BUILD_NUMBER%
ECHO - Docker Container is defined as %CONTAINER%
ECHO - Docker Registry is defined as %REGISTRY%
ECHO - Docker Image is defined as %IMAGE%
ECHO - Docker File is defined as %FILE%

ECHO Entering %WORKIN_DIR%
PUSHD "%WORKIN_DIR%"

ECHO *** Create docker test
SET BUILD_DOCKER_CMD="%EXE_DOCKER%" build -t %IMAGE% -f "%FILE%" "%PROJECT_ROOT_DIR%"
ECHO %BUILD_DOCKER_CMD%
%BUILD_DOCKER_CMD%

ECHO *** Create docker instance
SET CREATE_DOCKER_CMD="%EXE_DOCKER%" create --name %CONTAINER% %IMAGE%
ECHO %CREATE_DOCKER_CMD%
%CREATE_DOCKER_CMD%

ECHO *** Copy XML test reports
SET DOCKER_CP_CMD="%EXE_DOCKER%" cp %CONTAINER%:/opt/tests/efdhmtest.xml .
ECHO %DOCKER_CP_CMD%
%DOCKER_CP_CMD%
SET DOCKER_CP_CMD="%EXE_DOCKER%" cp %CONTAINER%:/opt/tests/gosctest.xml .
ECHO %DOCKER_CP_CMD%
%DOCKER_CP_CMD%

ECHO *** Remove docker instance
SET DOCKER_RM_CMD="%EXE_DOCKER%" rm %CONTAINER%
ECHO %DOCKER_RM_CMD%
%DOCKER_RM_CMD%

ECHO Leaving %WORKIN_DIR%
POPD

ECHO ---------------------------------------------------------------------------

REM Done
GOTO:EOF


::----------------------------------------------------------------------------------
:: Function declarations
:: Handy to read http://www.dostips.com/DtTutoFunctions.php for how dos functions
:: work.
::----------------------------------------------------------------------------------
:MakeAbsolute file base -- makes a file name absolute considering a base path
::                      -- file [in,out] - variable with file name to be converted, or file name itself for result in stdout
::                      -- base [in,opt] - base path, leave blank for current directory
:$created 20060101 :$changed 20080219 :$categories Path
:$source http://www.dostips.com
SETLOCAL ENABLEDELAYEDEXPANSION
set "src=%~1"
if defined %1 set "src=!%~1!"
set "bas=%~2"
if not defined bas set "bas=%cd%"
for /f "tokens=*" %%a in ("%bas%.\%src%") do set "src=%%~fa"
( ENDLOCAL & REM RETURN VALUES
    IF defined %1 (SET %~1=%src%) ELSE ECHO.%src%
)
EXIT /b

:inPath pathVar
::
::  Tests if the path stored within variable pathVar exists within PATH.
::
::  The result is returned as the ERRORLEVEL:
::    0 if the pathVar path is found in PATH.
::    1 if the pathVar path is not found in PATH.
::    2 if pathVar is missing or undefined or if PATH is undefined.
::
::  If the pathVar path is fully qualified, then it is logically compared
::  to each fully qualified path within PATH. The path strings don't have
::  to match exactly, they just need to be logically equivalent.
::
::  If the pathVar path is relative, then it is strictly compared to each
::  relative path within PATH. Case differences and double quotes are
::  ignored, but otherwise the path strings must match exactly.
::
::------------------------------------------------------------------------
::
:: Error checking
if "%~1"=="" exit /b 2
if not defined %~1 exit /b 2
if not defined path exit /b 2
::
:: Prepare to safely parse PATH into individual paths
setlocal DisableDelayedExpansion
set "var=%path:"=""%"
set "var=%var:^=^^%"
set "var=%var:&=^&%"
set "var=%var:|=^|%"
set "var=%var:<=^<%"
set "var=%var:>=^>%"
set "var=%var:;=^;^;%"
set var=%var:""="%
set "var=%var:"=""Q%"
set "var=%var:;;="S"S%"
set "var=%var:^;^;=;%"
set "var=%var:""="%"
setlocal EnableDelayedExpansion
set "var=!var:"Q=!"
set "var=!var:"S"S=";"!"
::
:: Remove quotes from pathVar and abort if it becomes empty
set "new=!%~1:"=!"
if not defined new exit /b 2
::
:: Determine if pathVar is fully qualified
echo("!new!"|findstr /i /r /c:^"^^\"[a-zA-Z]:[\\/][^\\/]" ^
                           /c:^"^^\"[\\][\\]" >nul ^
  && set "abs=1" || set "abs=0"
::
:: For each path in PATH, check if path is fully qualified and then do
:: proper comparison with pathVar.
:: Exit with ERRORLEVEL 0 if a match is found.
:: Delayed expansion must be disabled when expanding FOR variables
:: just in case the value contains !
for %%A in ("!new!\") do for %%B in ("!var!") do (
  if "!!"=="" endlocal
  for %%C in ("%%~B\") do (
    echo(%%B|findstr /i /r /c:^"^^\"[a-zA-Z]:[\\/][^\\/]" ^
                           /c:^"^^\"[\\][\\]" >nul ^
      && (if %abs%==1 if /i "%%~sA"=="%%~sC" exit /b 0) ^
      || (if %abs%==0 if /i "%%~A"=="%%~C" exit /b 0)
  )
)
:: No match was found so exit with ERRORLEVEL 1
exit /b 1
