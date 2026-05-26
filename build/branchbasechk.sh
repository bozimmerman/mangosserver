#!/bin/bash

git fetch origin

for branch in $(git branch | grep -v '^\*\? *master$' | tr -d ' *'); do
    echo "=== $branch ==="
    git log --oneline origin/master..$branch
    echo
done
