#include "fhe/multi_recursive_fhe.h"

MultiRecursiveFHE::MultiRecursiveFHE(const SEALContext &context, const SecretKey &sk, const FractalConfig &config)
    : context_(context), sk_(sk), config_(config), encoder_(context), decryptor_(context, sk)
{
    KeyGenerator kg(context, sk_);
    PublicKey pk;
    kg.create_public_key(pk);
    Encryptor encryptor(context, pk);
    std::vector<uint64_t> zero_vals(encoder_.slot_count(), 0ULL);
    Plaintext zero_pt;
    encoder_.encode(zero_vals, zero_pt);
    encryptor.encrypt(zero_pt, enc_zero_);
}

MultiRecursiveFHE::FractalCiphertext MultiRecursiveFHE::encrypt(const std::vector<uint64_t>& plaintext) {
    FractalCiphertext result;
    auto start = std::chrono::high_resolution_clock::now();
    
    Plaintext pt;
    encoder_.encode(plaintext, pt);
    KeyGenerator kg(context_, sk_);
    PublicKey pk;
    kg.create_public_key(pk);
    Encryptor encryptor(context_, pk);
    encryptor.encrypt(pt, result.data);
    result.stats.total_encryptions++;
    
    std::string data_str;
    for (auto v : plaintext) data_str += std::to_string(v) + ":";
    result.zkp = MultiRecursiveZKP::prove_recursive(data_str, 0, config_.recursion_depth, config_.zkp_branches);
    result.stats.total_zkp_proofs = result.zkp.total_proofs;
    
    if (config_.use_φ_convergence) {
        Evaluator evaluator(context_);
        for (size_t i = 0; i < config_.fhe_cycles; i++) {
            evaluator.add_inplace(result.data, enc_zero_);
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    result.stats.time_ms = std::chrono::duration<double, std::milli>(end - start).count();
    return result;
}

std::vector<uint64_t> MultiRecursiveFHE::decrypt_and_verify(FractalCiphertext& fct) {
    Plaintext pt;
    decryptor_.decrypt(fct.data, pt);
    std::vector<uint64_t> result;
    encoder_.decode(pt, result);
    result.resize(10);
    fct.stats.total_decryptions++;
    fct.stats.verified = MultiRecursiveZKP::verify_tree(fct.zkp);
    return result;
}

MultiRecursiveFHE::FractalCiphertext MultiRecursiveFHE::add(FractalCiphertext& a, FractalCiphertext& b) {
    FractalCiphertext result;
    Evaluator evaluator(context_);
    evaluator.add(a.data, b.data, result.data);
    return result;
}

MultiRecursiveFHE::FractalCiphertext MultiRecursiveFHE::multiply(FractalCiphertext& a, FractalCiphertext& b) {
    FractalCiphertext result;
    Evaluator evaluator(context_);
    evaluator.multiply(a.data, b.data, result.data);
    return result;
}
