@ECHO OFF

SETLOCAL ENABLEEXTENSIONS

SET PROJECT_ROOT_DIR=..\..
CALL:MakeAbsolute PROJECT_ROOT_DIR "%~dp0"

SET PROJECT_NAME=OPC UA Experiment
SET BUILD_NUMBER=1

SET EXE_DOCKER=docker.exe

ECHO ---------------------------------------------------------------------------
ECHO Docker build script for the %PROJECT_NAME%
ECHO %PROJECT_NAME% root directory is defined as %PROJECT_ROOT_DIR%

SET /p VERSION=<"%PROJECT_ROOT_DIR%\version.txt"
SET NUMBER=%VERSION%.%BUILD_NUMBER%

SET CONTAINER=opcuaex
SET REGISTRY=
REM FOR /f "delims=" %%x IN ("%PROJECT_ROOT_DIR%\version.txt") DO SET VERSION=%%x
REM SET IMAGE=%REGISTRY%/%CONTAINER%:%VERSION%-build
SET IMAGE=%CONTAINER%:%NUMBER%-build
SET FILE=%PROJECT_ROOT_DIR%\src\server\Dockerfile

REM ECHO - Work directory is defined as %WORKIN_DIR%
ECHO - Version is defined as %VERSION%
ECHO - Build number is defined as %BUILD_NUMBER%
ECHO - Docker Container is defined as %CONTAINER%
REM ECHO - Docker Registry is defined as %REGISTRY%
ECHO - Docker Image is defined as %IMAGE%
ECHO - Docker File is defined as %FILE%

ECHO Entering %PROJECT_ROOT_DIR%
PUSHD "%PROJECT_ROOT_DIR%"

ECHO *** Create docker build
SET BUILD_DOCKER_CMD="%EXE_DOCKER%" build -t %IMAGE% -f "%FILE%" "%PROJECT_ROOT_DIR%"
ECHO %BUILD_DOCKER_CMD%
%BUILD_DOCKER_CMD%

ECHO *** Create docker instance
SET CREATE_DOCKER_CMD="%EXE_DOCKER%" create --name %CONTAINER% %IMAGE%
ECHO %CREATE_DOCKER_CMD%
%CREATE_DOCKER_CMD%

REM ECHO *** Copy WASM Cache Busting
REM SET DOCKER_CP_CMD="%EXE_DOCKER%" cp %CONTAINER%:/opt/static/app/wasm-cache-busting.json .
REM ECHO %DOCKER_CP_CMD%
REM %DOCKER_CP_CMD%

REM ECHO *** Remove docker instance
REM SET DOCKER_RM_CMD="%EXE_DOCKER%" rm %CONTAINER%
REM ECHO %DOCKER_RM_CMD%
REM %DOCKER_RM_CMD%

ECHO Leaving %PROJECT_ROOT_DIR%
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
