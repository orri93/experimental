@ECHO OFF

SET EXPERIMENTAL_DIR=..
CALL:MakeAbsolute EXPERIMENTAL_DIR "%~dp0"

SET PROTOC_CMD=protoc
SET MVN_CMD=mvn

SET EXPERIMENTAL_JAVA_DIR=%EXPERIMENTAL_DIR%\java
SET EXPERIMENTAL_SPARKPLUGB_JAVA_DIR=%EXPERIMENTAL_JAVA_DIR%\sparkplugb

SET EXPERIMENTAL_EXTERN_TAHU_DIR=%EXPERIMENTAL_DIR%\extern\tahu

"%PROTOC_CMD%" --proto_path="%EXPERIMENTAL_EXTERN_TAHU_DIR%" --java_out="%EXPERIMENTAL_SPARKPLUGB_JAVA_DIR%\src\main\java" "%EXPERIMENTAL_EXTERN_TAHU_DIR%\sparkplug_b\sparkplug_b.proto"

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
