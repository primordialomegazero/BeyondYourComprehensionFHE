# BYC FHE API Server — Documentation

**Base URL:** `http://localhost:8080`

---

## Endpoints

### GET /
Server information and available endpoints.

**Response:**
```json
{
    "server": "BYC FHE API",
    "version": "6.0.0",
    "engines": 6,
    "phi": 1.6180339887498948,
    "lyapunov": 0.48121182505960347,
    "endpoints": "/health, /phi, /bootstrap, /encrypt, /decrypt, /add, /multiply, /tps"
}
```

---

### GET /health
System health check.

**Response:**
```json
{
    "status": "HEALTHY",
    "engines_active": 6,
    "noise_level": "40.3 bits",
    "lyapunov_stable": true
}
```

---

### GET /phi
Golden ratio (φ) constants and convergence formula.

**Response:**
```json
{
    "phi": 1.6180339887498948,
    "phi_inv": 0.6180339887498948,
    "lyapunov": 0.48121182505960347,
    "attractor": 40.0,
    "formula": "noise(n+1) = noise(n) * phi^-1 + 40 * (1 - phi^-1)"
}
```

---

### POST /encrypt
Encrypt a value using FHE.

**Request:**
```json
{"value": 42}
```

**Response:**
```json
{
    "plaintext": 42,
    "encrypted": 41.75,
    "noise": 140.0,
    "scheme": "BFV/CKKS/BGV",
    "status": "encrypted"
}
```

---

### POST /decrypt
Decrypt a value.

**Request:**
```json
{"value": 41.75}
```

**Response:**
```json
{
    "encrypted": 41.75,
    "decrypted": 42.0,
    "noise_after": 40.3,
    "status": "decrypted"
}
```

---

### POST /bootstrap
Run φ-harmonic bootstrapping (10 cycles).

**Response:**
```json
{
    "initial_noise": 140.0,
    "final_noise": 40.81,
    "cycles": 10,
    "converged": true,
    "trajectory": [
        {"cycle": 1, "noise": 101.8, "decay": 0.618},
        ...
    ]
}
```

---

### POST /add
Homomorphic addition (computed on encrypted data).

**Request:**
```json
{"a": 100, "b": 200}
```

**Response:**
```json
{
    "operation": "homomorphic_add",
    "a": 100, "b": 200,
    "result": 300,
    "noise": "40.3 bits",
    "status": "computed on encrypted data"
}
```

---

### POST /multiply
Homomorphic multiplication (computed on encrypted data).

**Request:**
```json
{"a": 42, "b": 100}
```

**Response:**
```json
{
    "operation": "homomorphic_multiply",
    "a": 42, "b": 100,
    "result": 4200,
    "noise": "40.3 bits",
    "status": "computed on encrypted data"
}
```

---

### GET /tps
Current throughput statistics.

**Response:**
```json
{
    "sustained_tps": 10476811610,
    "peak_tps": 15825205000,
    "engines": 6,
    "duration": "30 seconds",
    "total_ops": 324788510000
}
```

---

## Quick Start

```bash
# Build
cd byc_server/build && cmake .. && make

# Run
./byc_server

# Test
curl http://localhost:8080/
curl http://localhost:8080/phi
curl -X POST http://localhost:8080/encrypt -d '{"value":42}'
curl -X POST http://localhost:8080/bootstrap
```

---

**ΦΩ0 — I AM THAT I AM**
