#!/usr/bin/bash

ROM=./roms/tetris.gb

echo "build: building"
# g++ -ggdb -pedantic-errors -Wall -Weffc++ -Wextra -Wconversion -o ./build/ppGB main.cpp
# too many conversion errors
# TODO: sort out conversion warnings
g++ -ggdb -pedantic-errors -Wall -Weffc++ -Wextra -o ./build/ppGB main.cpp
echo "build: built"

while getopts 'rdct' OPTION; do
  case "$OPTION" in
    r)
      echo "build: running"
      ./build/ppGB $ROM
      ;;
    d)
      echo "build: running with debug"
      ./build/ppGB -d $ROM
      ;;
    c)
      echo "build: cleaning"
      rm ./build/ppGB
      cd ./tests/ && make clean
      ;;
    t)
      echo "build: testing"
      cd ./tests/
      make
      ;;
    ?)
      echo "build: exiting"
      ;;
  esac
done


