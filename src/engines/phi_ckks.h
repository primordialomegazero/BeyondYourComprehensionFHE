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
    constexpr double PHI = 1.6180339887498948482;

#ifdef HAS_SEAL
    class PhiCKKS {
        std::shared_ptr<seal::SEALContext> ctx;
        std::unique_ptr<seal::Encryptor> enc;
        std::unique_ptr<seal::Decryptor> dec;
        std::unique_ptr<seal::Evaluator> eval;
        std::unique_ptr<seal::CKKSEncoder> encoder;
        double scale_;

    public:
        bool ready = false;

        PhiCKKS() {
            try {
                seal::EncryptionParameters parms(seal::scheme_type::ckks);
                size_t poly_modulus_degree = 32768;  // 32,768 slots!
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

        // CKKS: 32,768 slots with decimal support!
        int slot_count() { return encoder->slot_count(); }  // 32,768!
    };
#endif
}
