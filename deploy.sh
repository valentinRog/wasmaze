#!/bin/sh

git checkout deploy
git merge --strategy=ours main
./build.sh
git add -f dist
git commit -m deploy
git push --set-upstream origin deploy
git checkout main