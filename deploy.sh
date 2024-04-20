#!/bin/sh

git checkout -b deploy
./build.sh
git add dist
git commit -m deploy
git push
git checkout main