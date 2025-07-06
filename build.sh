#!/bin/bash

mkdir -p bin

gcc -g -Wall -std=c99 \
  src/*.c src/system/*.c src/map/*.c src/gameloop/* \
  -I/usr/include \
  -I./lib/raylib/include \
  -I./src/headers \
  -I./src/lib/raylib/include \
  -lraylib -lm -ldl -lpthread -o bin/game

if [ $? -eq 0 ]; then
  echo "Compilation succeeded, running ./bin/game ..."
  ./bin/game
else
  echo "Compilation failed."
fi
