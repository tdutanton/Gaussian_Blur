#!/bin/bash

exePath="build/gaussian_blur"

if [ ! -f "$exePath" ]; then
    echo "File $exePath needed. Build first."
    exit 1
fi

echo "Running..."
cd "build" || exit
"./gaussian_blur"