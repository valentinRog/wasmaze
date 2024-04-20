#!/bin/sh

mkdir -p dist
cp *.js dist/
cp index.html dist/
cp style.css dist/
em++ -O3 -std=c++20 -I include *.cpp src/**.cpp -o dist/game.js -s WASM=1 -s -lembind -s ALLOW_MEMORY_GROWTH=1
