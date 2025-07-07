gcc -g -Wall -std=c99 \
  src/*.c src/system/*.c src/map/*.c src/gameloop/* src/gameobjects/* \
  -I/usr/local/include \
  -I/opt/homebrew/include \
  -I./lib/raylib/include \
  -I./src/headers \
  -L/usr/local/lib \
  -I/Users/skye/Downloads/raygui-4.0/src \
  -L/opt/homebrew/lib \
  -lraylib -lm -ldl -lpthread -o bin/game && ./bin/game
