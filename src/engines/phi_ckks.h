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
    class PhiCKKS {
        std::shared_ptr<seal::SEALContext> ctx;
        std::unique_ptr<seal::Encryptor> enc;
        std::unique_ptr<seal::Decryptor> dec;
        std::unique_ptr<seal::Evaluator> eval;
        std::unique_ptr<seal::CKKSEncoder> encoder;
        double scale_;

        std::string to_hex(const std::string& d) {
            std::ostringstream oss;
            for(unsigned char c : d) oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
            return oss.str();
        }
        std::string from_hex(const std::string& h) {
            std::string r;
            for(size_t i=0; i<h.length(); i+=2) r += (char)std::stoi(h.substr(i,2), nullptr, 16);
            return r;
        }

    public:
        bool ready = false;

        PhiCKKS() {
            try {
                seal::EncryptionParameters parms(seal::scheme_type::ckks);
                size_t poly_modulus_degree = 32768;
                parms.set_poly_modulus_degree(poly_modulus_degree);
                parms.set_coeff_modulus(seal::CoeffModulus::Create(poly_modulus_degree, {60, 40, 40, 60}));
                ctx = std::make_shared<seal::SEALContext>(parms);
                seal::KeyGenerator kg(*ctx);
                auto sk = kg.secret_key();
                seal::PublicKey pk; kg.create_public_key(pk);
                enc = std::make_unique<seal::Encryptor>(*ctx, pk);
                dec = std::make_unique<seal::Decryptor>(*ctx, sk);
                eval = std::make_unique<seal::Evaluator>(*ctx);
                encoder = std::make_unique<seal::CKKSEncoder>(*ctx);
                scale_ = pow(2.0, 40);
                ready = true;
            } catch(...) { ready = false; }
        }

        int slot_count() { return ready ? encoder->slot_count() : 0; }

        // Encrypt a single decimal value
        std::string encrypt_decimal(double value) {
            if (!ready) return "";
            seal::Plaintext pt;
            encoder->encode(value, scale_, pt);
            seal::Ciphertext ct;
            enc->encrypt(pt, ct);
            std::stringstream ss; ct.save(ss);
            return to_hex(ss.str());
        }

        // Decrypt to a decimal value

        std::string encrypt_batch(const std::vector<double>& values) {
            if (!ready) return "";
            seal::Plaintext pt;
            encoder->encode(values, scale_, pt);
            seal::Ciphertext ct;
            enc->encrypt(pt, ct);
            std::stringstream ss; ct.save(ss);
            return to_hex(ss.str());
        }

        std::vector<double> decrypt_batch(const std::string& hex_ct) {
            if (!ready) return {};
            std::string raw = from_hex(hex_ct);
            std::stringstream ss(raw);
            seal::Ciphertext ct;
            ct.load(*ctx, ss);
            seal::Plaintext pt;
            dec->decrypt(ct, pt);
            std::vector<double> out;
            encoder->decode(pt, out);
            return out;
        }
        double decrypt_decimal(const std::string& hex_ct) {
            if (!ready) return 0.0;
            std::string raw = from_hex(hex_ct);
            std::stringstream ss(raw);
            seal::Ciphertext ct;
            ct.load(*ctx, ss);
            seal::Plaintext pt;
            dec->decrypt(ct, pt);
            std::vector<double> out;
            encoder->decode(pt, out);
            return out.empty() ? 0.0 : out[0];
        }
    };
#endif
}
