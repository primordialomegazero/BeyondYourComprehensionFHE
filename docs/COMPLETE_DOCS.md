# BYC-FHE: Complete Documentation Index
## v22.3.0 — "Beyond Your Comprehension"

---

## Core Documents
- [README.md](../README.md) — Project overview, quick start, benchmarks
- [FORMAL_PROOFS.md](../proofs/FORMAL_PROOFS.md) — Mathematical proofs (6 theorems)
- [CHANGELOG.md](../CHANGELOG.md) — Version history

## Source Code

### Core Engine
- `src/core/banach_engine.h` — Banach contraction engine (from FEmmg v21.5)
- `src/core/femmg_fhe.h` — FHE operations (encrypt, decrypt, add, multiply)

### 9 Demon Engines
- `src/chaos/golden_chaos.h` — Gate 1: Golden Ratio Chaos
- `src/chaos/riemann_chaos.h` — Gate 2: Riemann Zeta Chaos
- `src/chaos/fibonacci_duel.h` — Gate 3: Fibonacci Duel (Triple Rashomon)
- `src/chaos/godel_incompleteness.h` — Gate 4: Gödel Incompleteness
- `src/chaos/cantor_diagonal.h` — Gate 5: Cantor Diagonal
- `src/chaos/turing_halting.h` — Gate 6: Turing Halting
- `src/chaos/heisenberg_uncertainty.h` — Gate 7: Heisenberg Uncertainty
- `src/chaos/nietzsche_eternal.h` — Gate 8: Nietzsche Eternal Return
- `src/chaos/schrodinger_cat.h` — Gate 9: Schrödinger's Cat

### Integration
- `src/chaos/demon_harmonizer.h` — 9-engine harmonizer (3 per op, Triple Rashomon)
- `src/chaos/eight_demon_gates.h` — 8 Demon Gates (4 per op, 70 combinations)

### Math & Security
- `src/math/phi_constants.h` — φ constants, Fibonacci, Riemann zeros
- `src/security/security_complete.h` — CSPRNG hardening, perturbation seed

## Tests
- `tests/test_1m_demons.cpp` — 1M ops benchmark (default: `make`)
- `tests/test_10m_demons.cpp` — 10M ops benchmark
- `tests/test_eight_demon_gates.cpp` — 8 Demon Gates integration test
- `tests/test_godel.cpp` — Gödel engine standalone test
- `tests/test_cantor.cpp` — Cantor engine standalone test
- `tests/test_turing.cpp` — Turing engine standalone test
- `tests/test_heisenberg.cpp` — Heisenberg engine standalone test
- `tests/test_nietzsche.cpp` — Nietzsche engine standalone test
- `tests/test_eight_gates.cpp` — 8 Gates comprehensive test

## Benchmarks
- `logs/1m_demons_results.txt` — 1M ops results (34,831 TPS, 9 engines)
- `logs/10m_demons_results.txt` — 10M ops results

## Distribution
- `npm-package/` — NPM: `@primordialomegazero/byc-fhe@22.3.0`
- `Dockerfile` — Docker: `ghcr.io/primordialomegazero/byc-fhe:v22.3.0`
- `Makefile` — Build system (default: 1M benchmark)

## Key Metrics (v22.3.0)
| Metric | Value |
|--------|-------|
| TPS (1 engine) | 120,885 |
| TPS (3 engines) | 34,831 |
| Total Engines | 9 |
| Engines per Op | 3 (randomized) |
| Engine Distribution | ~11.1% each |
| 8 Gates Combinations | 70 (8 choose 4) |
| Max Layers (8 Gates) | 56 |
| Max Chaos Output | 1.14 × 10⁴¹ |

---

*Last updated: July 2, 2026*  
*Author: Dan Joseph M. Fernandez / Primordial Omega Zero*
