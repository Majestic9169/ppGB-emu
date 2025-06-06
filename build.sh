#!/usr/bin/bash

echo "build: building"
g++ -ggdb -pedantic-errors -Wall -Weffc++ -Wextra -Wconversion -o ppGB main.cpp
echo "build: built"

while getopts 'rdc' OPTION; do
  case "$OPTION" in
    r)
      echo "build: running"
      ./ppGB ../gb-test-roms/cpu_instrs/cpu_instrs.gb
      ;;
    d)
      echo "build: running with debug"
      ./ppGB -d ../gb-test-roms/cpu_instrs/cpu_instrs.gb
      ;;
    c)
      echo "build: cleaning"
      rm ppGB
      ;;
    ?)
      echo "build: exiting"
      ;;
  esac
done


