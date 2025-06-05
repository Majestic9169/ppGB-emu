#!/usr/bin/bash

echo "building"
g++ -ggdb -pedantic-errors -Wall -Weffc++ -Wextra -Wconversion -o ppGB main.cpp
echo "built"

while getopts 'rdc' OPTION; do
  case "$OPTION" in
    r)
      echo "running"
      ./ppGB example.gb
      ;;
    d)
      echo "running with debug"
      ./ppGB -d example.gb
      ;;
    c)
      echo "cleaning"
      rm ppGB
      ;;
    ?)
      echo "exiting"
      ;;
  esac
done


