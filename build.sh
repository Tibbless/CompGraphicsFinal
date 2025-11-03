#!/bin/bash

# Build script for Eerie City

echo "=== Building Eerie City ==="

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

cd build

# Run CMake
echo "Running CMake..."
cmake .. || { echo "CMake failed!"; exit 1; }

# Build
echo "Building..."
make || { echo "Build failed!"; exit 1; }

echo ""
echo "=== Build Complete ==="
echo "Run with: ./build/EerieCity"
echo ""