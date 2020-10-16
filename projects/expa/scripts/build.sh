#!/bin/bash

GOS_PROJECT_NAME="Geirmundur Orri Sigurdsson Experiment Project A"

GOS_CMAKE=cmake
GOS_CTEST=ctest

die() {
  printf '%s\n' "$1" >&2
  exit 1
}

show_help() {
cat << EOF
Usage: ${0##*/} [-h] [-nc]
Build the ${GOS_PROJECT_NAME}

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
      GOS_NOT_CLEAN=NOT_CLEAN
      ;;
    -nb|--no-build|--not-build)
      GOS_NOT_BUILD=NOT_BUILD
      ;;
    -nd|--no_doc|--not-doc|--no_docs|--not-docs)
      GOS_BUILD_DOCS=OFF
      ;;
    -n|--build-number)
      GOS_BUILD_NUMBER=$2
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

#GOS_ROOT_DIR=`readlink -f "$DIR/.."`
GOS_ROOT_DIR=`realpath "$DIR/.."`

echo "---------------------------------------------------------------------------"
echo "Build script for the ${GOS_PROJECT_NAME}"
echo "${GOS_PROJECT_NAME} root directory is defined as ${GOS_ROOT_DIR}"

GOS_BUILD_CONFIG=release

GOS_TIER_B=${GOS_ROOT_DIR}/bin/tierb

GOS_PROJECT_BUILD_DIR=${GOS_ROOT_DIR}/build/${GOS_BUILD_CONFIG}
GOS_PROJECT_ARTIFACTS_DIR=${GOS_ROOT_DIR}/artifacts/${GOS_BUILD_CONFIG}
GOS_PROJECT_INCLUDE_DIR=${GOS_ROOT_DIR}/include

GOS_CMAKE_SYSTEM=Ninja
GOS_CMAKE_CREATE_OPTIONS="-G ${GOS_CMAKE_SYSTEM}"
GOS_CMAKE_CREATE_OPTIONS="${GOS_CMAKE_CREATE_OPTIONS} -DCMAKE_INSTALL_PREFIX:PATH=${GOS_PROJECT_ARTIFACTS_DIR}"
GOS_CMAKE_CREATE_OPTIONS="${GOS_CMAKE_CREATE_OPTIONS} -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=On"
GOS_CMAKE_CREATE_OPTIONS="${GOS_CMAKE_CREATE_OPTIONS} -DCMAKE_PLATFORM_INDEPENDENT_CODE:BOOL=On"
GOS_CMAKE_CREATE_OPTIONS="${GOS_CMAKE_CREATE_OPTIONS} -DCMAKE_BUILD_TYPE=${GOS_BUILD_CONFIG}"
GOS_CMAKE_CREATE_OPTIONS="${GOS_CMAKE_CREATE_OPTIONS} -DBUILD_TESTS:BOOL=On"
GOS_CMAKE_CREATE_OPTIONS="${GOS_CMAKE_CREATE_OPTIONS} -DBUILD_WITH_EMSCRIPTEN:BOOL=On"
GOS_CMAKE_CREATE_OPTIONS="${GOS_CMAKE_CREATE_OPTIONS} -DBUILD_WITH_EMSCRIPTEN:BOOL=On"
GOS_CMAKE_CREATE_OPTIONS="${GOS_CMAKE_CREATE_OPTIONS} ${GOS_ROOT_DIR}"

echo "- GOS Build number is defined as ${GOS_BUILD_NUMBER}"
echo "- Install path is defined as ${GOS_PROJECT_ARTIFACTS_DIR}"
echo "- CMake build system is defined as ${GOS_CMAKE_SYSTEM}"
echo "- CMake build directory is defined as ${GOS_PROJECT_BUILD_DIR}"
echo "- CMake build configuration is defined as ${GOS_BUILD_CONFIG}"

GOS_CMAKE_BUILD_OPTIONS="--build ${GOS_PROJECT_BUILD_DIR} --target all --config ${GOS_BUILD_CONFIG}"
GOS_CMAKE_INSTALL_OPTIONS="--build ${GOS_PROJECT_BUILD_DIR} --target install --config ${GOS_BUILD_CONFIG}"
GOS_CTEST_OPTIONS="--build-config ${GOS_BUILD_CONFIG}"

if [[ $GOS_NOT_CLEAN = "NOT_CLEAN" ]]; then
  echo "Skipping Clean"
else
  echo "*** Clean"
  if [ -d ${GOS_PROJECT_BUILD_DIR} ]; then
    echo "The build folder already exists so deleting the old"
    ${GOS_CMAKE} -E remove_directory ${GOS_PROJECT_BUILD_DIR}
  fi
  if [ -d ${GOS_PROJECT_ARTIFACTS_DIR} ]; then
    echo "The artifacts folder already exists so deleting the old"
    ${GOS_CMAKE} -E remove_directory ${GOS_PROJECT_ARTIFACTS_DIR}
  fi
fi

echo "Creating a build folder ${GOS_PROJECT_BUILD_DIR}"
${GOS_CMAKE} -E make_directory ${GOS_PROJECT_BUILD_DIR}

echo "*** Creating a CMake Build"
GOS_CMAKE_CREATE_BUILD_CMD="${GOS_CMAKE} -E chdir ${GOS_PROJECT_BUILD_DIR} ${GOS_CMAKE} ${GOS_CMAKE_CREATE_OPTIONS}"
echo "${GOS_CMAKE_CREATE_BUILD_CMD}"
${GOS_CMAKE_CREATE_BUILD_CMD}

if [[ $GOS_NOT_BUILD = "NOT_BUILD" ]]; then
  echo "Skipping Building"
else
  echo "*** CMake Building"
  GOS_CMAKE_BUILD_CMD="${GOS_CMAKE} ${GOS_CMAKE_BUILD_OPTIONS}"
  echo "${GOS_CMAKE_BUILD_CMD}"
  ${GOS_CMAKE_BUILD_CMD}
fi

echo "*** CTest Testing"
GOS_CMAKE_CTEST_CMD="${GOS_CMAKE} -E chdir ${GOS_PROJECT_BUILD_DIR} ${GOS_CTEST} ${GOS_CTEST_OPTIONS}"
echo "${GOS_CMAKE_CTEST_CMD}"
${GOS_CMAKE_CTEST_CMD}

echo "*** CMake Installing"
GOS_CMAKE_INSTALL_CMD="${GOS_CMAKE} ${GOS_CMAKE_INSTALL_OPTIONS}"
echo "${GOS_CMAKE_INSTALL_CMD}"
${GOS_CMAKE_INSTALL_CMD}

echo "*** Coping libraries"
GOS_CP_LIB_CMD="cp ${GOS_PROJECT_ARTIFACTS_DIR}/lib/*.a ${GOS_ROOT_DIR}/lib/"
echo "${GOS_CP_LIB_CMD}"
${GOS_CP_LIB_CMD}

echo "Entering ${GOS_ROOT_DIR}"
silent_pushd ${GOS_ROOT_DIR}

echo "*** Generate WebAssembly Cache for the Angular Web"
GOS_NPM_WASM_CMD="npm run wasm"
echo "${GOS_NPM_WASM_CMD}"
${GOS_NPM_WASM_CMD}

echo "*** Build Angular Web"
GOS_NG_BUILD_CMD="ng build --prod"
echo "${GOS_NG_BUILD_CMD}"
${GOS_NG_BUILD_CMD}

echo "*** Build Go Tier"
GOS_GO_BUILD_CMD="go build -o ${GOS_TIER_B} ${GOS_ROOT_DIR}/src/tierb/tier.go"
echo "${GOS_GO_BUILD_CMD}"
${GOS_GO_BUILD_CMD}

echo "Leaving ${GOS_ROOT_DIR}"
silent_popd

echo "---------------------------------------------------------------------------"
