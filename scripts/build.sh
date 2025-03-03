#!/bin/sh

BUILD_DIR="build"

show_help() {
  echo "Usage: ./build.sh [OPTIONS]"
  echo "Build and clean your project."
  echo
  echo "Options:"
  echo "  -h, --help         Show this help message and exit."
  echo "  -c, --clean        Delete build folders."
  echo "  -es, --examples    Build all examples"
  echo "  -e, --example <ex> Build example <ex>"
}

case "$1" in
  -c | --clean)
    echo "Deleting build folders"
    rm -rf $BUILD_DIR output *.db
    exit 0
    ;;
  -e | --example)
    if [ -z $2 ];
    then
      echo "Provide the name of the Example"
      exit 0
    else
      cmake -S . -B $BUILD_DIR  -DBUILD_EXAMPLES=yes -DCMAKE_BUILD_TYPE=Debug
    fi
    ;;
  -es | --examples)
    cmake -S . -B $BUILD_DIR  -DBUILD_EXAMPLES=yes -DCMAKE_BUILD_TYPE=Debug
    ;;
  -h | --help)
    show_help
    exit 0 
    ;;
  *)
    mkdir -p $BUILD_DIR
    cmake -S . -B $BUILD_DIR 
    ;;
esac

case "$1" in
  -e | --example)
    cmake --build $BUILD_DIR --target $2 
    ;;
  *)
    cmake --build $BUILD_DIR
    ;;
esac
