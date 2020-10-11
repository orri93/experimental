#!/bin/bash

GOS_PROJECT_NAME="Geirmundur Orri Sigurdsson Experiment Project A"

die() {
  printf '%s\n' "$1" >&2
  exit 1
}

show_help() {
cat << EOF
Usage: ${0##*/} [-h] [-nc]
Initialize the ${GOS_PROJECT_NAME}

     -h --help       display this help and exit
     -c --clean
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

GOS_ROOT_DIR=`realpath "$DIR/.."`

echo "---------------------------------------------------------------------------"
echo "Initialize script for the ${GOS_PROJECT_NAME}"
echo "${GOS_PROJECT_NAME} root directory is defined as ${GOS_ROOT_DIR}"

echo "*** node install"
GOS_NODE_INIT_CMD="npm install"
echo "${GOS_NODE_INIT_CMD}"
${GOS_NODE_INIT_CMD}

echo "*** go get"
GOS_GO_GET_CMD="go get gopkg.in/yaml.v3"
echo "${GOS_GO_GET_CMD}"
${GOS_GO_GET_CMD}
GOS_GO_GET_CMD="go get github.com/gorilla/mux"
echo "${GOS_GO_GET_CMD}"
${GOS_GO_GET_CMD}
GOS_GO_GET_CMD="go get github.com/gorilla/websocket"
echo "${GOS_GO_GET_CMD}"
${GOS_GO_GET_CMD}

echo "---------------------------------------------------------------------------"
