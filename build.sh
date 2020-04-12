#!/bin/bash

catch_errors() {
  printf "\nbuild.sh failed!\n" >&2
  exit 1
}

trap catch_errors ERR;

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd)

usage() {
  cat <<EOF
  build.sh [OPTIONS] Builds the project using the supplied options.

  EXAMPLE:
    build.sh --build_type=Debug

  OPTIONS:
  -h
    --help                 this help text

  --build_type=BUILD_TYPE  The build type to use (Debug | Release)
EOF
}

build_type="Debug"

while [ "$1" != "" ]; do
  PARAM=`echo $1 | awk -F= '{print $1}'`
  VALUE=`echo $1 | awk -F= '{print $2}'`
  case $PARAM in
    -h | --help)
      usage
      exit 0
      ;;
    --build_type)
      build_type=$VALUE
      ;;
    *)
      echo "ERROR: unknown parameter \"$PARAM\""
      usage
      exit 1
      ;;
  esac
  shift
done

cd $DIR

./format.sh

if [ ! -d build ]; then
  mkdir build
fi

cd build

cmake -DCMAKE_BUILD_TYPE=$build_type -G "Unix Makefiles" ..
cmake --build . -- -j$(nproc)

cd $PREV_DIR

exit 0

