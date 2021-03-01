@ECHO OFF

SETLOCAL ENABLEEXTENSIONS

SET PROJECT_ROOT_DIR=..\..
CALL:MakeAbsolute PROJECT_ROOT_DIR "%~dp0"

SET PROJECT_NAME=GOS WebAssembly D3 Heatmap Charting Experiment Modules

ECHO ---------------------------------------------------------------------------
ECHO Emscripten Build script for the %PROJECT_NAME%
ECHO %PROJECT_NAME% root directory is defined as %PROJECT_ROOT_DIR%

IF "%EMSDK%" == "" GOTO no_emsdk
GOTO check_emsdk_done
:no_emsdk
ECHO The EMSDK environmental variable is undefined. Emscripten compiler toolchain has not been enabled 1>&2
EXIT /b
:check_emsdk_done

SET WORKIN_DIR=%PROJECT_ROOT_DIR%\src\assets\wasm

SET PROJECT_INCLUDE=%PROJECT_ROOT_DIR%\include
SET PROJECT_SRC_DIR=%PROJECT_ROOT_DIR%\src\modules
SET GOSC_DIR=%PROJECT_ROOT_DIR%\extern\gosc
SET GOSC_INCLUDE=%GOSC_DIR%\include
SET GOSC_SRC_DIR=%GOSC_DIR%\src

ECHO Entering %WORKIN_DIR%
PUSHD "%WORKIN_DIR%"

ECHO *** Building WebAssembly modules with Emscripten
SET EMCC_CMD=call emcc -I"%PROJECT_INCLUDE%" -I"%GOSC_INCLUDE%" -O3 -s MODULARIZE=1 -s TOTAL_MEMORY=67108864 -s ASSERTIONS=0 -s EXPORT_NAME="HmModule" -s EXTRA_EXPORTED_RUNTIME_METHODS=['ccall'] -o hm.js "%GOSC_SRC_DIR%/interpolate.c" "%GOSC_SRC_DIR%/geometry.c" "%GOSC_SRC_DIR%/screen.c" "%GOSC_SRC_DIR%/color.c" "%GOSC_SRC_DIR%/scale.c" "%GOSC_SRC_DIR%/text.c" "%PROJECT_SRC_DIR%/hm/hmm.c" "%PROJECT_SRC_DIR%/hm/hm.c"
ECHO %EMCC_CMD%
%EMCC_CMD%
SET EMCC_CMD=call emcc -I"%PROJECT_INCLUDE%" -I"%GOSC_INCLUDE%" -O3 -s MODULARIZE=1 -s TOTAL_MEMORY=67108864 -s ASSERTIONS=0 -s EXPORT_NAME="DemoModule" -s EXTRA_EXPORTED_RUNTIME_METHODS=['ccall'] -o demo.js "%GOSC_SRC_DIR%/interpolate.c" "%GOSC_SRC_DIR%/screen.c" "%GOSC_SRC_DIR%/color.c" "%GOSC_SRC_DIR%/text.c" "%PROJECT_SRC_DIR%/demo/demo.c"
ECHO %EMCC_CMD%
%EMCC_CMD%

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
