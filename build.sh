#!/usr/bin/bash

ROM=./roms/tetris.gb

mkdir -p ./build

echo "build: building"
# g++ -ggdb -pedantic-errors -Wall -Weffc++ -Wextra -Wconversion -o ./build/ppGB main.cpp
# removed -Wconversion because too many conversion errors
# TODO: sort out conversion warnings
cd ./build/ && cmake .. && make && cd ../
echo "build: built"

while getopts 'rdct' OPTION; do
  case "$OPTION" in
    r)
      echo "build: running"
      ./build/ppGB-emu $ROM
      ;;
    d)
      echo "build: running with debug"
      ./build/ppGB-emu -d $ROM
      ;;
    c)
      echo "build: cleaning"
      rm -r ./build/*
      ;;
    t)
      echo "build: testing"
      cd ./build/ && make tests && cd ../
      ./build/tests --reporter compact
      ;;
    ?)
      echo "build: exiting"
      ;;
  esac
done


