#!/bin/bash

echo "Establishing project structure..."

# make the build directory
if [ ! -d build/c/objects/ ]; then
    mkdir -p build/objects/c
fi

# make the results directory
if [ ! -d results/ ]; then
    mkdir -p results/
fi
