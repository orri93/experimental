#!/bin/bash

GOS_PROJECT_NAME="Geirmundur Orri Sigurdsson Experiment Project A"

die() {
  printf '%s\n' "$1" >&2
  exit 1
}

show_help() {
cat << EOF
Usage: ${0##*/} [-h] [-nc]
Start the ${GOS_PROJECT_NAME}

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
    -c|--clean)
      echo "Clean detected"
      GOS_CLEAN=CLEAN
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

GOS_ROOT_DIR=`realpath "$DIR/.."`

echo "---------------------------------------------------------------------------"
echo "Start script for the ${GOS_PROJECT_NAME}"
echo "${GOS_PROJECT_NAME} root directory is defined as ${GOS_ROOT_DIR}"


echo "Entering ${GOS_ROOT_DIR}"
silent_pushd ${GOS_ROOT_DIR}

echo "*** Start the Go service"
GOS_START_CMD="${GOS_ROOT_DIR}/bin/tierb --config ${GOS_ROOT_DIR}/etc/configuration.yaml --web ${GOS_ROOT_DIR}/dist/expa"
echo "${GOS_START_CMD}"
${GOS_START_CMD}

echo "Leaving ${GOS_ROOT_DIR}"
silent_popd

echo "---------------------------------------------------------------------------"

