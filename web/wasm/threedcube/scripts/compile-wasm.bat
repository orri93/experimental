@ECHO OFF

SETLOCAL ENABLEEXTENSIONS

SET GOS_PROJECT_NAME=Compile Web Assembly 3D-Cube Angular Example

SET GOS_PROJECT_ROOT_DIR=..
CALL:MakeAbsolute GOS_PROJECT_ROOT_DIR "%~dp0"

ECHO %GOS_PROJECT_NAME% root path is defined as %GOS_PROJECT_ROOT_DIR%

IF DEFINED EMSDK GOTO gos_emsdk_defined
ECHO EMSDK is not defined, please make sure emsdk_env has been called 1>&2
EXIT /b 1
:gos_emsdk_defined
ECHO EMSDK defined as %EMSDK%

IF DEFINED EMSDK_NODE GOTO gos_emsdk_node_defined
ECHO EMSDK_NODE is not defined, please make sure emsdk_env has been called 1>&2
EXIT /b 1
:gos_emsdk_node_defined
ECHO EMSDK_NODE defined as %EMSDK_NODE%

SET GOS_PROJECT_SRC_DIR=%GOS_PROJECT_ROOT_DIR%\src
SET GOS_PROJECT_APP_DIR=%GOS_PROJECT_SRC_DIR%\app
SET GOS_PROJECT_ASSET_DIR=%GOS_PROJECT_SRC_DIR%\assets
SET GOS_PROJECT_WASM_ASSET_DIR=%GOS_PROJECT_ASSET_DIR%\wasm

ECHO Enter %GOS_PROJECT_ROOT_DIR%
PUSHD "%GOS_PROJECT_ROOT_DIR%"
ECHO Execute emcc compiler commands

SET GOS_EMCC_OPTIONS=-Os "%GOS_PROJECT_APP_DIR%\three-dee-cube\soil\libSOIL.bc" "%GOS_PROJECT_APP_DIR%\three-dee-cube\3d-cube.c" -o "%GOS_PROJECT_WASM_ASSET_DIR%\3d-cube.js" -s LEGACY_GL_EMULATION=1 -Isrc/app/three-dee-cube/soil -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']" -s FORCE_FILESYSTEM=1 -s MODULARIZE=1 -s EXPORT_NAME="Cube3dModule"
SET GOS_EMCC_CMD=call emcc %GOS_EMCC_OPTIONS%
ECHO %GOS_EMCC_CMD%
%GOS_EMCC_CMD%

SET GOS_EMCC_OPTIONS=-Os "%GOS_PROJECT_APP_DIR%\sdl-cube\sdl-cube.c" -o "%GOS_PROJECT_WASM_ASSET_DIR%\sdl-cube.js" -s LEGACY_GL_EMULATION=1 -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']" -s MODULARIZE=1 -s EXPORT_NAME="CubeSdlModule"
SET GOS_EMCC_CMD=call emcc %GOS_EMCC_OPTIONS%
ECHO %GOS_EMCC_CMD%
%GOS_EMCC_CMD%

SET GOS_EMCC_OPTIONS=-Os "%GOS_PROJECT_APP_DIR%\sdl-chart\sdl-chart.c" -o "%GOS_PROJECT_WASM_ASSET_DIR%\sdl-chart.js" -s LEGACY_GL_EMULATION=1 -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']" -s MODULARIZE=1 -s EXPORT_NAME="ChartSdlModule"
SET GOS_EMCC_CMD=call emcc %GOS_EMCC_OPTIONS%
ECHO %GOS_EMCC_CMD%
%GOS_EMCC_CMD%

ECHO Leave %GOS_PROJECT_ROOT_DIR%
POPD

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
EXIT /b 1

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