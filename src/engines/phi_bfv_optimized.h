#pragma once
#ifdef HAS_SEAL
#include <seal/seal.h>
#endif
#include <cmath>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>

namespace phi_seal {

#ifdef HAS_SEAL
    class PhiBFVOptimized {
    private:
        std::shared_ptr<seal::SEALContext> ctx;
        std::unique_ptr<seal::Encryptor> enc;
        std::unique_ptr<seal::Decryptor> dec;
        std::unique_ptr<seal::Evaluator> eval;
        std::unique_ptr<seal::BatchEncoder> benc;

        static std::string to_hex(const std::string& d) {
            std::ostringstream oss;
            for(unsigned char c : d) oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
            return oss.str();
        }
        static std::string from_hex(const std::string& h) {
            std::string r;
            for(size_t i=0; i<h.length(); i+=2) r += (char)std::stoi(h.substr(i,2), nullptr, 16);
            return r;
        }

    public:
        bool ready = false;

        PhiBFVOptimized() {
            try {
                seal::EncryptionParameters parms(seal::scheme_type::bfv);
                size_t poly_modulus_degree = 2048;
                parms.set_poly_modulus_degree(poly_modulus_degree);
                parms.set_coeff_modulus(seal::CoeffModulus::BFVDefault(poly_modulus_degree));
                parms.set_plain_modulus(seal::PlainModulus::Batching(poly_modulus_degree, 20));
                ctx = std::make_shared<seal::SEALContext>(parms);
                seal::KeyGenerator kg(*ctx);
                auto sk = kg.secret_key();
                seal::PublicKey pk; kg.create_public_key(pk);
                enc = std::make_unique<seal::Encryptor>(*ctx, pk);
                dec = std::make_unique<seal::Decryptor>(*ctx, sk);
                eval = std::make_unique<seal::Evaluator>(*ctx);
                benc = std::make_unique<seal::BatchEncoder>(*ctx);
                ready = true;
            } catch(...) { ready = false; }
        }

        int slot_count() { return ready ? benc->slot_count() : 0; }

        std::string encrypt_value(int64_t value) {
            if (!ready) return "";
            std::vector<int64_t> vals = {value};
            seal::Plaintext pt; benc->encode(vals, pt);
            seal::Ciphertext ct; enc->encrypt(pt, ct);
            std::stringstream ss; ct.save(ss);
            return to_hex(ss.str());
        }

        int64_t decrypt_value(const std::string& hex_ct) {
            if (!ready) return 0;
            std::string raw = from_hex(hex_ct);
            std::stringstream ss(raw);
            seal::Ciphertext ct; ct.load(*ctx, ss);
            seal::Plaintext pt; dec->decrypt(ct, pt);
            std::vector<int64_t> out; benc->decode(pt, out);
            return out.empty() ? 0 : out[0];
        }

        std::string add_ct(const std::string& hex_a, const std::string& hex_b) {
            if (!ready) return "";
            seal::Ciphertext ca, cb;
            std::stringstream sa(from_hex(hex_a)), sb(from_hex(hex_b));
            ca.load(*ctx, sa); cb.load(*ctx, sb);
            eval->add_inplace(ca, cb);
            std::stringstream ss; ca.save(ss);
            return to_hex(ss.str());
        }
    };
#endif
}
