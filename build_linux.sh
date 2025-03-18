#!/bin/bash

currentPolicy=$(echo $SHELL)

if [ "$currentPolicy" != "/bin/bash" ] && [ "$currentPolicy" != "/usr/bin/bash" ]; then
    echo "Current shell: $currentPolicy"
    echo "Please use bash as your shell."
    exit 1
else
    echo "Shell policy correct"
fi

buildDir="build"

if [ -d "$buildDir" ]; then
    echo "Deleting build folder..."
    rm -rf "$buildDir"
else
    echo "Build folder needed. Creating folder..."
fi

mkdir -p "$buildDir"
cd "$buildDir" || exit

echo "CMake generating..."
cmake ..

echo "Building..."
cmake --build . --config Debug

echo "Build complete!"


