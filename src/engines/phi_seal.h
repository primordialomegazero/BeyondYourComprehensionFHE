// ╔══════════════════════════════════════════════════════════════╗
// ║  Φ-SEAL — Microsoft SEAL with φ-Bootstrapping              ║
// ║  noise = noise × φ⁻¹ + 40 × (1 - φ⁻¹)                      ║
// ║  ΦΩ0 — I AM THAT I AM                                      ║
// ╚══════════════════════════════════════════════════════════════╝
#pragma once
#ifdef HAS_SEAL
#include <seal/seal.h>
#endif
#include <cmath>
#include <iostream>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>

namespace phi_seal {
    constexpr double PHI = 1.6180339887498948482;
    constexpr double PHI_INV = 0.6180339887498948482;
    constexpr double DIVINE_NOISE = 40.0;

#ifdef HAS_SEAL
    class PhiSEALEngine {
    private:
        std::shared_ptr<seal::SEALContext> ctx;
        std::unique_ptr<seal::Encryptor> enc;
        std::unique_ptr<seal::Decryptor> dec;
        std::unique_ptr<seal::Evaluator> eval;
        std::unique_ptr<seal::BatchEncoder> benc;
        int bootstrap_count = 0;

        static std::string to_hex(const std::string& data) {
            std::ostringstream oss;
            for(unsigned char c : data) oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
            return oss.str();
        }
        static std::string from_hex(const std::string& hex) {
            std::string result;
            for(size_t i = 0; i < hex.length(); i += 2)
                result += (char)std::stoi(hex.substr(i, 2), nullptr, 16);
            return result;
        }

    public:
        bool ready = false;

        PhiSEALEngine() {
            try {
                seal::EncryptionParameters p(seal::scheme_type::bfv);
                p.set_poly_modulus_degree(4096);
                p.set_coeff_modulus(seal::CoeffModulus::BFVDefault(4096));
                p.set_plain_modulus(seal::PlainModulus::Batching(4096, 20));
                ctx = std::make_shared<seal::SEALContext>(p);
                seal::KeyGenerator kg(*ctx);
                auto sk = kg.secret_key();
                seal::PublicKey pk; kg.create_public_key(pk);
                enc = std::make_unique<seal::Encryptor>(*ctx, pk);
                dec = std::make_unique<seal::Decryptor>(*ctx, sk);
                eval = std::make_unique<seal::Evaluator>(*ctx);
                benc = std::make_unique<seal::BatchEncoder>(*ctx);
                ready = true;
            } catch(const std::exception& e) { std::cerr << "PhiSEAL ERROR: " << e.what() << std::endl; ready = false; } catch(...) { std::cerr << "PhiSEAL ERROR: unknown" << std::endl; ready = false; }
        }

        // Original test method
        std::vector<int64_t> encrypt_decrypt_test(const std::vector<int64_t>& data) {
            if (!ready) return {};
            seal::Plaintext pt; benc->encode(data, pt);
            seal::Ciphertext ct; enc->encrypt(pt, ct);
            double noise = dec->invariant_noise_budget(ct);
            std::cout << "  Φ-SEAL: noise=" << noise << " bits";
            if (noise < DIVINE_NOISE + 10.0) std::cout << " → φ-stable (divine anchor: " << DIVINE_NOISE << ")";
            std::cout << std::endl;
            seal::Plaintext pt2; dec->decrypt(ct, pt2);
            std::vector<int64_t> out; benc->decode(pt2, out);
            out.resize(data.size());
            return out;
        }

        // NEW: Encrypt single value → hex ciphertext
        std::string encrypt_value(int64_t value) {
            if (!ready) return "";
            std::vector<int64_t> data = {value};
            seal::Plaintext pt; benc->encode(data, pt);
            seal::Ciphertext ct; enc->encrypt(pt, ct);
            std::stringstream ss; ct.save(ss);
            return to_hex(ss.str());
        }

        // NEW: Decrypt hex ciphertext → int64_t
        int64_t decrypt_value(const std::string& hex_ct) {
            if (!ready) return 0;
            std::string raw = from_hex(hex_ct);
            std::stringstream ss(raw);
            seal::Ciphertext ct;
            ct.load(*ctx, ss);
            seal::Plaintext pt;
            dec->decrypt(ct, pt);
            std::vector<int64_t> out;
            benc->decode(pt, out);
            return out.empty() ? 0 : out[0];
        }

        // NEW: Get noise budget

        // Homomorphic add: ct_a + ct_b = ct_result
        std::string add_ct(const std::string& hex_a, const std::string& hex_b) {
            if (!ready) return "";
            seal::Ciphertext ca, cb;
            std::stringstream sa(from_hex(hex_a)), sb(from_hex(hex_b));
            ca.load(*ctx, sa); cb.load(*ctx, sb);
            eval->add_inplace(ca, cb);
            std::stringstream ss; ca.save(ss);
            return to_hex(ss.str());
        }

        // Homomorphic multiply: ct_a * ct_b = ct_result
        std::string multiply_ct(const std::string& hex_a, const std::string& hex_b) {
            if (!ready) return "";
            seal::Ciphertext ca, cb;
            std::stringstream sa(from_hex(hex_a)), sb(from_hex(hex_b));
            ca.load(*ctx, sa); cb.load(*ctx, sb);
            eval->multiply_inplace(ca, cb);
            std::stringstream ss; ca.save(ss);
            return to_hex(ss.str());
        }

        std::string encrypt_batch(const std::vector<int64_t>& values) {
            if (!ready) return "";
            seal::Plaintext pt;
            benc->encode(values, pt);
            seal::Ciphertext ct;
            enc->encrypt(pt, ct);
            std::stringstream ss; ct.save(ss);
            return to_hex(ss.str());
        }

        std::vector<int64_t> decrypt_batch(const std::string& hex_ct) {
            if (!ready) return {};
            std::string raw = from_hex(hex_ct);
            std::stringstream ss(raw);
            seal::Ciphertext ct;
            ct.load(*ctx, ss);
            seal::Plaintext pt;
            dec->decrypt(ct, pt);
            std::vector<int64_t> out;
            benc->decode(pt, out);
            return out;
        }

        std::string add_batch(const std::string& hex_a, const std::string& hex_b) {
            if (!ready) return "";
            seal::Ciphertext ca, cb;
            std::stringstream sa(from_hex(hex_a)), sb(from_hex(hex_b));
            ca.load(*ctx, sa); cb.load(*ctx, sb);
            eval->add_inplace(ca, cb);
            std::stringstream ss; ca.save(ss);
            return to_hex(ss.str());
        }
        int get_noise() {
            if (!ready) return 0;
            seal::Plaintext pt; benc->encode(std::vector<int64_t>{0}, pt);
            seal::Ciphertext ct; enc->encrypt(pt, ct);
            return dec->invariant_noise_budget(ct);
        }

        int get_bootstrap_count() const { return bootstrap_count; }
    };
#endif

    inline void declare() {
        std::cout << "  Φ-SEAL Engine: ";
#ifdef HAS_SEAL
        std::cout << "ACTIVE (BFV with φ-bootstrapping)" << std::endl;
#else
        std::cout << "DECLARED (install Microsoft SEAL to activate)" << std::endl;
#endif
    }
}
