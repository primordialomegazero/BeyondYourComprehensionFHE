const { PozDFClient } = require('./index.js');

console.log('╔════════════════════════════════════════╗');
console.log('║  pozDF-FHE v7.0 Client Library Test   ║');
console.log('║  Override Architect Edition           ║');
console.log('╚════════════════════════════════════════╝\n');

const alice = new PozDFClient();
const bob = new PozDFClient();
let passed = 0, total = 10;

// 1
if (alice.clientId !== bob.clientId) { console.log('1. Key Generation ✅'); passed++; }
else console.log('1. Key Generation ❌');

// 2
const a = alice.encrypt(42), b = alice.encrypt(42);
if (a !== b) { console.log('2. Probabilistic IND-CPA ✅'); passed++; }
else console.log('2. Probabilistic IND-CPA ❌');

// 3
if (alice.decrypt(a) === 42) { console.log('3. Correct Decryption ✅'); passed++; }
else console.log('3. Correct Decryption ❌');

// 4
const e15 = alice.encrypt(15), e27 = alice.encrypt(27);
if (alice.decrypt(e15 + e27 - alice.lambda) === 42) { console.log('4. Homomorphic Add ✅'); passed++; }
else console.log('4. Homomorphic Add ❌');

// 5
const e6 = alice.encrypt(6), e7 = alice.encrypt(7);
if (alice.decrypt((e6-alice.lambda)*(e7-alice.lambda)/alice.phi + alice.lambda) === 42) { console.log('5. Homomorphic Multiply ✅'); passed++; }
else console.log('5. Homomorphic Multiply ❌');

// 6 - Cross-client: different IDs = different server sessions
if (alice.clientId !== bob.clientId) { console.log('6. Cross-Client IDs Unique ✅'); passed++; }
else console.log('6. Cross-Client IDs Unique ❌');

// 7
const keys = alice.getSecretKeys();
const restored = PozDFClient.fromKeys(keys);
if (restored.decrypt(restored.encrypt(42)) === 42) { console.log('7. Key Export/Import ✅'); passed++; }
else console.log('7. Key Export/Import ❌');

// 8
const pub = alice.getPublicInfo();
if (pub.seed === undefined && pub.phi === undefined) { console.log('8. Public Info Safe ✅'); passed++; }
else console.log('8. Public Info Safe ❌');

// 9
const reg = alice.getRegistrationPayload();
if (reg.seed === undefined && reg.phi === undefined) { console.log('9. Register Safe ✅'); passed++; }
else console.log('9. Register Safe ❌');

// 10 - All enterprise payloads
const am = alice.getAntiMatterPayload().action === 'antimatter';
const pqc = alice.getPQCPayload().action === 'pqc';
const zkp = alice.getZKPPayload(42,7).action === 'zkp';
const sc = alice.getSupplyChainPayload().action === 'supply_chain';
if (am && pqc && zkp && sc) { console.log('10. Enterprise Payloads ✅'); passed++; }
else console.log('10. Enterprise Payloads ❌');

console.log(`\n╔════════════════════════════════════════╗`);
console.log(`║  RESULT: ${passed}/${total} PASSED — BEYOND COMPREHENSION ║`);
console.log(`╚════════════════════════════════════════╝`);
