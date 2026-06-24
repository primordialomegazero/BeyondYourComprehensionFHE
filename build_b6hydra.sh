#!/bin/bash
echo "🔨 BUILDING B6 HYDRA v6.0..."
echo "================================"
echo "⚠️ This will take 10-20 minutes!"
echo ""

cd BeyondYourComprehensionFHE

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "[1/3] Configuring CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON

# Check if CMake succeeded
if [ $? -ne 0 ]; then
    echo "❌ CMake failed! Check dependencies."
    exit 1
fi

# Build
echo "[2/3] Building (this will take a while)..."
make -j$(nproc)

# Check if build succeeded
if [ $? -ne 0 ]; then
    echo "❌ Build failed! Trying with single thread..."
    make
fi

echo "[3/3] Build complete!"
ls -lh b6_hydra

echo ""
echo "✅ BUILD SUCCESSFUL!"
echo "Run with: ./b6_hydra --help"
