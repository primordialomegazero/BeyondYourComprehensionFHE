#!/bin/bash
cd BeyondYourComprehensionFHE
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release 2>&1 | tee cmake_output.log
make -j$(nproc) 2>&1 | tee make_output.log
ls -la b6_hydra hydra_gateway 2>/dev/null || echo "Executables not found"
