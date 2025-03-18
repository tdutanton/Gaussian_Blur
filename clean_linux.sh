#!/bin/bash

buildDir="build"
tempFiles=("CMakeCache.txt" "CMakeFiles" "*.user" "*.obj" "*.log")

if [ -d "$buildDir" ]; then
    echo "Removing $buildDir..."
    rm -rf "$buildDir"
    echo -e "\e[32mFolder $buildDir removed.\e[0m"
fi

for file in "${tempFiles[@]}"; do
    matches=$(find . -type f -name "$file" -o -type d -name "$file")
    if [ -n "$matches" ]; then
        while IFS= read -r match; do
            echo "Removing $match ..."
            rm -rf "$match"
        done <<< "$matches"
    fi
done

echo -e "\e[36mClean done.\e[0m"