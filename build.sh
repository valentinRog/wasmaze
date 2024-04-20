#!/bin/sh

mkdir -p dist
em++ -O3 -std=c++20 -I include *.cpp -o dist/game.js -s WASM=1 -s -lembind -s ALLOW_MEMORY_GROWTH=1
