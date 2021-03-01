@ECHO OFF

SETLOCAL ENABLEEXTENSIONS

SET PROJECT_ROOT_DIR=..
CALL:MakeAbsolute PROJECT_ROOT_DIR "%~dp0"

SET PROJECT_NAME=GOS WebAssembly D3 Heatmap Charting Experiment

SET EXE_CMAKE=cmake.exe
SET EXE_CTEST=ctest.exe

ECHO ---------------------------------------------------------------------------
ECHO Build script for the %PROJECT_NAME%
ECHO %PROJECT_NAME% root directory is defined as %PROJECT_ROOT_DIR%

REM Set build variables
SET BUILD_CONFIG=RelWithDebInfo

IF "%EMSDK%" == "" GOTO no_emsdk
GOTO check_emsdk_done
:no_emsdk
ECHO The EMSDK environmental variable is undefined. Emscripten compiler toolchain has not been enabled 1>&2
EXIT /b
:check_emsdk_done

IF "%SDL_LIB%" == "" GOTO no_sdl_lib
GOTO check_sdl_lib_done
:no_sdl_lib
ECHO The SDL_LIB environmental variable is undefined. The build script needs to know where to find the SDL library.
EXIT /b
:check_sdl_lib_done

IF "%1" == "noclean" GOTO not_clean
IF "%1" == "notclean" GOTO not_clean
IF "%1" == "Noclean" GOTO not_clean
IF "%1" == "Notclean" GOTO not_clean
IF "%1" == "noClean" GOTO not_clean
IF "%1" == "notClean" GOTO not_clean
IF "%1" == "NoClean" GOTO not_clean
IF "%1" == "NotClean" GOTO not_clean
GOTO check_not_clean_done
:not_clean
ECHO Not Clean detected
SET NOT_CLEAN=NotClean
SHIFT
:check_not_clean_done

IF "%1" == "nobuild" GOTO not_build
IF "%1" == "nobuilds" GOTO not_build
IF "%1" == "notbuild" GOTO not_build
IF "%1" == "notbuilds" GOTO not_build
IF "%1" == "noBuild" GOTO not_build
IF "%1" == "noBuilds" GOTO not_build
IF "%1" == "notBuild" GOTO not_build
IF "%1" == "notBuilds" GOTO not_build
IF "%1" == "NoBuild" GOTO not_build
IF "%1" == "NoBuilds" GOTO not_build
IF "%1" == "NotBuild" GOTO not_build
IF "%1" == "NotBuilds" GOTO not_build
SET BUILDING=ON
GOTO no_build_done
:not_build
ECHO No Build detected
SET BUILDING=OFF
SHIFT
:no_build_done

IF "%1" == "" GOTO no_build_number
SET BUILD_NUMBER=%1
GOTO build_number_done
:no_build_number
SET BUILD_NUMBER=0
:build_number_done
SHIFT

REM Check for Python
REM SET PYTHON_IN_PATH_INDICATOR=C:\Python38\
REM CALL:inPath PYTHON_IN_PATH_INDICATOR && ( GOTO python_in_path ) || ( GOTO python_not_in_path )
REM :python_in_path
REM ECHO The Python bin folders is already in the path
REM GOTO python_in_path_done
REM :python_not_in_path
REM ECHO Putting the Python bin folders into the path
REM SET PATH=%PATH%;C:\Python38\;C:\Python38\Scripts\;C:\%USERPROFILE%\AppData\Roaming\Python\Scripts
REM :python_in_path_done

SET PROJECT_BUILD_DIR=%PROJECT_ROOT_DIR%\build\%BUILD_CONFIG%
SET PROJECT_ARTIFACTS_DIR=%PROJECT_ROOT_DIR%\artifacts\%BUILD_CONFIG%
SET PROJECT_INCLUDE_DIR=%PROJECT_ROOT_DIR%\include
REM SET EMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR=%EMSDK%\upstream\emscripten\system\include
SET SDL_INCLUDE_DIR=%SDL_LIB%\include
SET SDL_LIBRARY=%SDL_LIB%\VisualC\x64\Debug\SDL.lib

SET CMAKE_SYSTEM=Visual Studio 16 2019
REM SET CMAKE_SYSTEM=MinGW Makefiles
REM SET CMAKE_PLATFORM=x64

REM -DBUILD_DOCS:BOOL=%BUILD_DOCS% ^
REM -DBUILD_NUMBER=%BUILD_NUMBER% ^
REM -Dgtest_force_shared_crt:BOOL=ON ^
REM -DCMAKE_BUILD_TYPE=%BUILD_CONFIG% ^
REM -DDEPLOY_PDB_FILES:BOOL=Off ^
REM -DC_EMSCRIPTEN_BUILD:BOOL=On ^
REM -A %CMAKE_PLATFORM% ^
REM --graphviz="%PROJECT_ARTIFACTS_DIR%\share\graphviz\expa" ^
REM -DEMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR:PATH="%EMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR%" ^
REM --log-level=DEBUG ^

REM -DUSE_CLOCK_GETTIME:BOOL=On ^
REM -DEXPA_EMSCRIPTEN_BUILD:BOOL=On ^

SET CMAKE_CREATE_OPTIONS=^
-DCMAKE_INSTALL_PREFIX:PATH="%PROJECT_ARTIFACTS_DIR%" ^
-DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=On ^
-DCMAKE_PLATFORM_INDEPENDENT_CODE:BOOL=On ^
-DEM_TARGET:BOOL=On ^
-DBUILD_TESTS:BOOL=On ^
-DSDL_INCLUDE_DIR:PATH="%SDL_INCLUDE_DIR%" ^
-DSDL_LIBRARY:FILE="%SDL_LIBRARY%" ^
-G "%CMAKE_SYSTEM%" ^
"%PROJECT_ROOT_DIR%"

ECHO - GOS Build number is defined as %BUILD_NUMBER%
ECHO - Install path is defined as "%PROJECT_ARTIFACTS_DIR%"
ECHO - CMake buld system is defined as "%CMAKE_SYSTEM%"
REM ECHO - CMake buld platform is defined as "%CMAKE_PLATFORM%"
ECHO - CMake buld directory is defined as "%PROJECT_BUILD_DIR%"
ECHO - CMake buld configuration is defined as "%BUILD_CONFIG%"

ECHO - The EMSDK environmental variable defined as "%EMSDK%"
ECHO - The SDL_LIB environmental variable defined as "%SDL_LIB%"

REM SET CMAKE_BUILD_OPTIONS=--build "%PROJECT_BUILD_DIR%" --target ALL_BUILD --config %BUILD_CONFIG%
SET CMAKE_BUILD_OPTIONS=--build "%PROJECT_BUILD_DIR%" --config %BUILD_CONFIG%
SET CMAKE_INSTALL_OPTIONS=--build "%PROJECT_BUILD_DIR%" --target install --config %BUILD_CONFIG%

SET CTEST_OPTIONS=--build-config %BUILD_CONFIG%

IF "%NOT_CLEAN%" == "NotClean" GOTO do_not_clean
IF EXIST "%PROJECT_BUILD_DIR%" (
  ECHO The build folder already exists so deleting the old
  "%EXE_CMAKE%" -E remove_directory "%PROJECT_BUILD_DIR%"
)
IF EXIST "%PROJECT_ARTIFACTS_DIR%" (
  ECHO The artifacts folder already exists so deleting the old
  "%EXE_CMAKE%" -E remove_directory "%PROJECT_ARTIFACTS_DIR%"
)
:do_not_clean

ECHO Creating a build folder %PROJECT_BUILD_DIR%
"%EXE_CMAKE%" -E make_directory "%PROJECT_BUILD_DIR%"

ECHO *** Creating a Build
SET CMAKE_CREATE_BUILD_CMD="%EXE_CMAKE%" -E chdir "%PROJECT_BUILD_DIR%" "%EXE_CMAKE%" %CMAKE_CREATE_OPTIONS%
ECHO %CMAKE_CREATE_BUILD_CMD%
%CMAKE_CREATE_BUILD_CMD%

IF "%BUILDING%" == "OFF" GOTO do_not_build
ECHO *** Building
SET CMAKE_BUILD_CMD="%EXE_CMAKE%" %CMAKE_BUILD_OPTIONS%
ECHO %CMAKE_BUILD_CMD%
%CMAKE_BUILD_CMD%

ECHO *** Testing
SET CMAKE_CTEST_CMD="%EXE_CMAKE%" -E chdir "%PROJECT_BUILD_DIR%" "%EXE_CTEST%" %CTEST_OPTIONS%
ECHO %CMAKE_CTEST_CMD%
%CMAKE_CTEST_CMD%
:do_not_build

ECHO *** Installing
SET CMAKE_INSTALL_CMD="%EXE_CMAKE%" %CMAKE_INSTALL_OPTIONS%
ECHO %CMAKE_INSTALL_CMD%
%CMAKE_INSTALL_CMD%

ECHO *** Coping libraries
SET CP_LIB_CMD=xcopy "%PROJECT_ARTIFACTS_DIR%\lib\*.a" "%PROJECT_ROOT_DIR%\lib\" /Y
ECHO %CP_LIB_CMD%
%CP_LIB_CMD%
SET CP_LIB_CMD=xcopy "%PROJECT_ARTIFACTS_DIR%\lib\*.js" "%PROJECT_ROOT_DIR%\src\assets\wasm" /Y
ECHO %CP_LIB_CMD%
%CP_LIB_CMD%
SET CP_LIB_CMD=xcopy "%PROJECT_ARTIFACTS_DIR%\lib\*.wasm" "%PROJECT_ROOT_DIR%\src\assets\wasm" /Y
ECHO %CP_LIB_CMD%
%CP_LIB_CMD%
REM SET CP_LIB_CMD=xcopy "%PROJECT_ARTIFACTS_DIR%\lib\*.data" "%PROJECT_ROOT_DIR%\src\assets\wasm" /Y
REM ECHO %CP_LIB_CMD%
REM %CP_LIB_CMD%

ECHO Entering %PROJECT_ROOT_DIR%
PUSHD "%PROJECT_ROOT_DIR%"

ECHO *** Generate WebAssembly Cache for the Angular Web
SET NPM_WASM_CMD=call npm run wasm
ECHO %NPM_WASM_CMD%
%NPM_WASM_CMD%

ECHO *** Build Angular Web
SET NG_BUILD_CMD=call ng build --prod
ECHO %NG_BUILD_CMD%
%NG_BUILD_CMD%

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
