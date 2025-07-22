#!/usr/bin/bash

ROM=./roms/rom.gb

mkdir -p ./build

echo "build: building"
# g++ -ggdb -pedantic-errors -Wall -Weffc++ -Wextra -Wconversion -o ./build/ppGB main.cpp
# removed -Wconversion because too many conversion errors
# TODO: sort out conversion warnings

if (( $OPTIND == 1 )); then
    cd ./build/ && cmake .. && make && cd ../
    echo "build: built"
fi

while getopts 'rdctp' OPTION; do
  case "$OPTION" in
    r)
      cd ./build/ && cmake .. && make && cd ../
      echo "build: built"
      echo "build: running"
      ./build/ppGB-emu $ROM
      ;;
    d)
      cd ./build/ && cmake .. && make && cd ../
      echo "build: built"
      echo "build: running with debug"
      ./build/ppGB-emu -d $ROM
      ;;
    c)
      echo "build: cleaning"
      rm -r ./build/*
      ;;
    t)
      echo "build: testing"
      cd ./build/
      cmake ..
      make tests 
      cd ..
      ./build/tests --reporter compact --success
      ;;
    p) 
      echo "build: testing"
      cd ./build/ 
      cmake .. -DCOVERAGE=ON
      find . -name "*.gcda" -exec rm {} \;
      make tests
      cd ..
      ./build/tests --reporter compact
      find . -name "*.gcda" | xargs gcov -H -k -q -s "./gcov/"
      find . -name "*.gcda" -exec rm {} \;
      find . -name "*.gcov" -exec mv -f {} ./gcov/ 2>/dev/null \;
      ;;
    ?)
      echo "build: exiting"
      ;;
  esac
done


