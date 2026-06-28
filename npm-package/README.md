# pozDF-FHE Client Library v7.0

**Override Architect Edition** — Zero-Knowledge Fully Homomorphic Encryption with Anti-Matter Shield, 8 PQC, Fractal ZKP, and Supply Chain Security.

## Install

```bash
npm install @primordialomegazero/pozd-fhe-client
```

## Quick Start

```javascript
const { PozDFClient } = require('@primordialomegazero/pozd-fhe-client');
const client = new PozDFClient();

// Encrypt locally (keys NEVER sent to server)
const enc15 = client.encrypt(15);
const enc27 = client.encrypt(27);

// Register with blind server
await fetch('http://localhost:8093/', {
  method: 'POST',
  body: JSON.stringify(client.getRegistrationPayload())
});

// Send encrypted data for blind computation
const resp = await fetch('http://localhost:8093/', {
  method: 'POST',
  body: JSON.stringify(client.getAddPayload(enc15, enc27))
});
const { encrypted_result } = await resp.json();

// Decrypt locally
console.log(client.decrypt(encrypted_result)); // 42
```

## Enterprise Features

- 🔒 **Zero-Knowledge** — Server never sees keys
- 🎲 **IND-CPA** — Probabilistic chaotic nonce
- 🛡️ **Anti-Matter Shield** — Schumann resonance, Lyapunov anomaly detection
- 🔐 **8 PQC Algorithms** — Post-quantum ready
- 📐 **Fractal ZKP** — Zero-knowledge proofs
- 🔗 **Supply Chain** — Multi-recursive verification
- ⚡ **15M+ TPS** — On consumer hardware

## License

MIT — Dan Joseph M. Fernandez / Primordial Omega Zero
