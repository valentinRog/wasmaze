#!/bin/sh

mkdir -p dist
em++ -I include *.cpp src/**.cpp -o dist/game.js -s WASM=1 -s -lembind -s ALLOW_MEMORY_GROWTH=1
