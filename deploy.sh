#!/bin/sh

git checkout deploy
./build.sh
git add dist
git commit -m deploy
git push --set-upstream origin deploy
git checkout main