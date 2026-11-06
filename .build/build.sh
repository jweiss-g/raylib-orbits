#!/bin/bash
cd "$(dirname "$0")/.."

mkdir -p ".bin"

set -e

clang src/main.c -o .bin/main \
  -I$(brew --prefix raylib)/include \
  -L$(brew --prefix raylib)/lib \
  -lraylib \
  -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

./.bin/main