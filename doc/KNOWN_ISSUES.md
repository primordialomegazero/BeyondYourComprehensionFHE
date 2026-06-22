# Known Issues

## Active

| Issue | Impact | Workaround |
|-------|--------|------------|
| **PQC Verification** | ML-DSA-87/Falcon verify fails (liboqs 0.15.0 bug) | Use φ-proof for verification |
| **Zama TFHE Engine** | Not yet integrated | Declared, Rust env needed |
| **TFHE-rs Engine** | Not yet integrated | Declared, Rust env needed |
| **Fractal ZKP Value Preservation** | Multi-layer loses value | Use single-layer TrueBootstrapper |

## Resolved

| Issue | Resolution |
|-------|------------|
| Value drift at 5M+ | Fixed with 30-bit plaintext modulus |
| Enc(0) precomputation | Auto-generated in constructor |
| Noise floor at 0 | φ-anchor at 40 bits |
