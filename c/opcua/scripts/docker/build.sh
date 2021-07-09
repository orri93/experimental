#!/bin/bash

PROJECT_NAME="OPC UA Experiment"

DOCKER=docker
BUILD_NUMBER=1

die() {
  printf '%s\n' "$1" >&2
  exit 1
}

show_help() {
cat << EOF
Usage: ${0##*/} [-h] [-nc]
Build the ${PROJECT_NAME} with docker

     -h --help       display this help and exit
     -b --build      the build number
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
    -b|--build)
      echo "Build detected"
      shift
      BUILD_NUMBER=$1
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

ROOT_DIR=`realpath "$DIR/../.."`

echo "---------------------------------------------------------------------------"
echo "Docker build script for the ${PROJECT_NAME}"
echo "${PROJECT_NAME} root directory is defined as ${ROOT_DIR}"

VERSION=`cat ${ROOT_DIR}/version.txt`
NUMBER="${VERSION}.${BUILD_NUMBER}"

CONTAINER="opcuaex"
# REGISTRY=""
# IMAGE="${REGISTRY}/${CONTAINER}:${NUMBER}-build"
IMAGE="${CONTAINER}:${NUMBER}-build"
FILE="${ROOT_DIR}/src/server/Dockerfile"

echo "- Version is defined as ${VERSION}"
echo "- Build number is defined as ${BUILD_NUMBER}"
echo "- Docker Container is defined as ${CONTAINER}"
# echo "- Docker Registry is defined as ${REGISTRY}"
echo "- Docker Image is defined as ${IMAGE}"
echo "- Docker File is defined as ${FILE}"

echo "Entering ${ROOT_DIR}"
silent_pushd ${ROOT_DIR}

echo "*** Create docker build"
BUILD_DOCKER_CMD="${DOCKER} build -t ${IMAGE} -f ${FILE} ${ROOT_DIR}"
echo "${BUILD_DOCKER_CMD}"
${BUILD_DOCKER_CMD}
if [ $? -eq 0 ]
  then
    echo "Creating docker test was successful"
  else
    echo "Creating docker test failed with result code $?"
    exit $?
fi

echo "*** Create docker instance"
CREATE_DOCKER_CMD="${DOCKER} create --name ${CONTAINER} ${IMAGE}"
echo "${CREATE_DOCKER_CMD}"
${CREATE_DOCKER_CMD}
if [ $? -ne 0 ] ; then exit $? ; fi

#echo "*** Remove docker instance"
#DOCKER_RM_CMD="${DOCKER} rm ${CONTAINER}"
#echo "${DOCKER_RM_CMD}"
#${DOCKER_RM_CMD}
#if [ $? -ne 0 ] ; then exit $? ; fi

echo "Leaving ${ROOT_DIR}"
silent_popd

echo "---------------------------------------------------------------------------"
