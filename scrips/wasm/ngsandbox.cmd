@ECHO OFF

SET EXPERIMENTAL_DIR=..\..
CALL:MakeAbsolute EXPERIMENTAL_DIR "%~dp0"

SET EMCC_CMD=emcc

SET EXPERIMENTAL_WEB_DIR=%EXPERIMENTAL_DIR%\web
SET EXPERIMENTAL_NG_SANDBOX_DIR=%EXPERIMENTAL_WEB_DIR%\ngsandbox

SET EXPERIMENTAL_NG_SANDBOX_SRC_DIR=%EXPERIMENTAL_NG_SANDBOX_DIR%\src
SET EXPERIMENTAL_NG_SANDBOX_ASSET_WASM_DIR=%EXPERIMENTAL_NG_SANDBOX_SRC_DIR%\assets\wasm

PUSHD "%EXPERIMENTAL_NG_SANDBOX_SRC_DIR%\wasm"
"%EMCC_CMD%" -Os "%EXPERIMENTAL_NG_SANDBOX_SRC_DIR%\wasm\fibonacci.c" -o "%EXPERIMENTAL_NG_SANDBOX_ASSET_WASM_DIR%\fibonacci.wasm" --no-entry
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
EXIT /b
