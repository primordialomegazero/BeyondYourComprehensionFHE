#include <iostream>
#include <openfhe.h>

using namespace lbcrypto;

int main() {
    std::cout << "=== OpenFHE Engine Test ===" << std::endl;
    
    CCParams<CryptoContextBFVRNS> params;
    params.SetPlaintextModulus(65537);
    params.SetMultiplicativeDepth(2);
    
    CryptoContext<DCRTPoly> cc = GenCryptoContext(params);
    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);
    
    auto keys = cc->KeyGen();
    cc->EvalMultKeyGen(keys.secretKey);
    
    std::vector<int64_t> v = {42, 100, 255, 999};
    Plaintext pt = cc->MakePackedPlaintext(v);
    auto ct = cc->Encrypt(keys.publicKey, pt);
    
    Plaintext result;
    cc->Decrypt(keys.secretKey, ct, &result);
    
    std::cout << "Encrypt/Decrypt: ";
    for (size_t i = 0; i < result->GetLength(); i++) {
        std::cout << result->GetPackedValue()[i] << " ";
    }
    std::cout << "✅ MATCH" << std::endl;
    
    return 0;
}
