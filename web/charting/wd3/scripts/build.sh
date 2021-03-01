#!/bin/bash

PROJECT_NAME="GOS WebAssembly D3 Heatmap Charting Experiment"

CMAKE=cmake
CTEST=ctest

die() {
  printf '%s\n' "$1" >&2
  exit 1
}

show_help() {
cat << EOF
Usage: ${0##*/} [-h] [-nc]
Build the ${PROJECT_NAME}

     -h --help       display this help and exit
     -v              verbose mode. Can be used multiple times for increased
                     verbosity.
EOF
}

while :; do
  case $1 in
    -h|-\?|--help)
      show_help      # Display a usage synopsis.
      exit
      ;;
    -nc|--no-clean|--not--clean)
      echo "Not Clean detected"
      NOT_CLEAN=NOT_CLEAN
      ;;
    -nb|--no-build|--not-build)
      NOT_BUILD=NOT_BUILD
      ;;
    -nd|--no_doc|--not-doc|--no_docs|--not-docs)
      BUILD_DOCS=OFF
      ;;
    -n|--build-number)
      BUILD_NUMBER=$2
      ;;
    --)              # End of all options.
      shift
      break
      ;;
    -?*)
      printf 'WARN: Unknown option (ignored): %s\n' "$1" >&2
      ;;
    *)               # Default case: No more options, so break out of the loop.
      break
  esac
  shift
done

# Resolve the Current Script Directory
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  TARGET="$(readlink "$SOURCE")"
  if [[ $TARGET == /* ]]; then
    echo "SOURCE '$SOURCE' is an absolute symlink to '$TARGET'"
    SOURCE="$TARGET"
  else
    DIR="$( dirname "$SOURCE" )"
    echo "SOURCE '$SOURCE' is a relative symlink to '$TARGET' (relative to '$DIR')"
    SOURCE="$DIR/$TARGET" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
  fi
done
echo "SOURCE is '$SOURCE'"
RDIR="$( dirname "$SOURCE" )"
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
if [ "$DIR" != "$RDIR" ]; then
  echo "DIR '$RDIR' resolves to '$DIR'"
fi
echo "DIR is '$DIR'"

# Silent pushd and popd
silent_pushd () {
    command pushd "$@" > /dev/null
}
silent_popd () {
    command popd "$@" > /dev/null
}

#ROOT_DIR=`readlink -f "$DIR/.."`
ROOT_DIR=`realpath "$DIR/.."`

# Fix spaces in folder name
ROOT_DIR=${ROOT_DIR//\ /\\\ }

# Check the environment
if [ -z "$EMSDK" ]
then
  printf 'ERROR: The EMSDK environment variable is undefined. Please enable Emscripten by runing source emsdk/emsdk_env.sh.\n' >&2
  exit 1
fi


echo "---------------------------------------------------------------------------"
echo "Build script for the ${PROJECT_NAME}"
echo "${PROJECT_NAME} root directory is defined as ${ROOT_DIR}"

BUILD_CONFIG=release

PROJECT_BUILD_DIR=${ROOT_DIR}/build/${BUILD_CONFIG}
PROJECT_ARTIFACTS_DIR=${ROOT_DIR}/artifacts/${BUILD_CONFIG}
PROJECT_INCLUDE_DIR=${ROOT_DIR}/include
# EMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR=${EMSDK}/upstream/emscripten/system/include

CMAKE_SYSTEM=Ninja
CMAKE_CREATE_OPTIONS="-G ${CMAKE_SYSTEM}"
CMAKE_CREATE_OPTIONS="${CMAKE_CREATE_OPTIONS} -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_ARTIFACTS_DIR}"
CMAKE_CREATE_OPTIONS="${CMAKE_CREATE_OPTIONS} -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=On"
CMAKE_CREATE_OPTIONS="${CMAKE_CREATE_OPTIONS} -DCMAKE_PLATFORM_INDEPENDENT_CODE:BOOL=On"
CMAKE_CREATE_OPTIONS="${CMAKE_CREATE_OPTIONS} -DCMAKE_BUILD_TYPE=${BUILD_CONFIG}"
# CMAKE_CREATE_OPTIONS="${CMAKE_CREATE_OPTIONS} -DEMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR:PATH=${EMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR}"
CMAKE_CREATE_OPTIONS="${CMAKE_CREATE_OPTIONS} -DEM_TARGET:BOOL=On"
CMAKE_CREATE_OPTIONS="${CMAKE_CREATE_OPTIONS} -DBUILD_TESTS:BOOL=On"
CMAKE_CREATE_OPTIONS="${CMAKE_CREATE_OPTIONS} ${ROOT_DIR}"

echo "- Build number is defined as ${BUILD_NUMBER}"
echo "- Install path is defined as ${PROJECT_ARTIFACTS_DIR}"
echo "- CMake build system is defined as ${CMAKE_SYSTEM}"
echo "- CMake build directory is defined as ${PROJECT_BUILD_DIR}"
echo "- CMake build configuration is defined as ${BUILD_CONFIG}"

CMAKE_BUILD_OPTIONS="--build ${PROJECT_BUILD_DIR} --target all --config ${BUILD_CONFIG}"
CMAKE_INSTALL_OPTIONS="--build ${PROJECT_BUILD_DIR} --target install --config ${BUILD_CONFIG}"
CTEST_OPTIONS="--build-config ${BUILD_CONFIG}"

if [[ $NOT_CLEAN = "NOT_CLEAN" ]]; then
  echo "Skipping Clean"
else
  echo "*** Clean"
  if [ -d ${PROJECT_BUILD_DIR} ]; then
    echo "The build folder already exists so deleting the old"
    ${CMAKE} -E remove_directory ${PROJECT_BUILD_DIR}
  fi
  if [ -d ${PROJECT_ARTIFACTS_DIR} ]; then
    echo "The artifacts folder already exists so deleting the old"
    ${CMAKE} -E remove_directory ${PROJECT_ARTIFACTS_DIR}
  fi
fi

echo "Creating a build folder ${PROJECT_BUILD_DIR}"
${CMAKE} -E make_directory ${PROJECT_BUILD_DIR}

echo "*** Creating a CMake Build"
CMAKE_CREATE_BUILD_CMD="${CMAKE} -E chdir ${PROJECT_BUILD_DIR} ${CMAKE} ${CMAKE_CREATE_OPTIONS}"
echo "${CMAKE_CREATE_BUILD_CMD}"
${CMAKE_CREATE_BUILD_CMD}

if [[ $NOT_BUILD = "NOT_BUILD" ]]; then
  echo "Skipping Building"
else
  echo "*** CMake Building"
  CMAKE_BUILD_CMD="${CMAKE} ${CMAKE_BUILD_OPTIONS}"
  echo "${CMAKE_BUILD_CMD}"
  ${CMAKE_BUILD_CMD}
fi

echo "*** CTest Testing"
CMAKE_CTEST_CMD="${CMAKE} -E chdir ${PROJECT_BUILD_DIR} ${CTEST} ${CTEST_OPTIONS}"
echo "${CMAKE_CTEST_CMD}"
${CMAKE_CTEST_CMD}

echo "*** CMake Installing"
CMAKE_INSTALL_CMD="${CMAKE} ${CMAKE_INSTALL_OPTIONS}"
echo "${CMAKE_INSTALL_CMD}"
${CMAKE_INSTALL_CMD}

echo "*** Coping libraries"
CP_LIB_CMD="cp ${PROJECT_ARTIFACTS_DIR}/lib/*.js ${ROOT_DIR}/src/assets/wasm/"
echo "${CP_LIB_CMD}"
${CP_LIB_CMD}
CP_LIB_CMD="cp ${PROJECT_ARTIFACTS_DIR}/lib/*.wasm ${ROOT_DIR}/src/assets/wasm/"
echo "${CP_LIB_CMD}"
${CP_LIB_CMD}
# CP_LIB_CMD="cp ${PROJECT_ARTIFACTS_DIR}/lib/*.data ${ROOT_DIR}/src/assets/wasm/"
# echo "${CP_LIB_CMD}"
# ${CP_LIB_CMD}

echo "Entering ${ROOT_DIR}"
silent_pushd ${ROOT_DIR}

echo "*** Generate WebAssembly Cache for the Angular Web"
NPM_WASM_CMD="npm run wasm"
echo "${NPM_WASM_CMD}"
${NPM_WASM_CMD}

echo "*** Build Angular Web"
NG_BUILD_CMD="ng build --prod"
echo "${NG_BUILD_CMD}"
${NG_BUILD_CMD}

echo "Leaving ${ROOT_DIR}"
silent_popd

echo "---------------------------------------------------------------------------"
