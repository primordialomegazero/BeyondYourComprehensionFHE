# Contributing to BeyondYourComprehensionFHE

## Quick Start
```bash
git clone https://github.com/primordialomegazero/BeyondYourComprehensionFHE.git
cd BeyondYourComprehensionFHE
mkdir build && cd build
cmake .. -DSEAL_DIR=/usr/local/lib/cmake/SEAL-4.1
make -j$(nproc)
./b6_hydra
```

## Pull Request Guidelines
- All tests must pass (`make test`)
- New features need test coverage
- Keep the φ-framework philosophy
- No breaking changes to existing API

## Code Style
- C++17
- Follow SEAL naming conventions
- φ-constants must use `1.6180339887498948482`

## Issues
Found a bug? Open an issue with:
- SEAL version
- Compiler version
- Steps to reproduce
- Expected vs actual behavior

## License
MIT — Contributions agree to same license.
