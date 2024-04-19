#!/bin/sh

EXPORTED_FUNCTIONS='["_init"]'

mkdir -p dist
em++ -I include *.cpp src/**.cpp -o dist/game.js -s WASM=1 -s -lembind -s EXPORTED_RUNTIME_METHODS='["cwrap"]'
