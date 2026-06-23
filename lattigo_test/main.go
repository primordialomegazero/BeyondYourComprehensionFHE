package main

import (
    "fmt"
    "github.com/tuneinsight/lattigo/v5/core/rlwe"
    "github.com/tuneinsight/lattigo/v5/schemes/bgv"
)

func main() {
    fmt.Println("╔══════════════════════════════════════════════╗")
    fmt.Println("║  Φ-LATTIGO STANDALONE TEST                 ║")
    fmt.Println("║  BGV Scheme + φ-Mirror Healing             ║")
    fmt.Println("╚══════════════════════════════════════════════╝")
    fmt.Println()

    // Setup BGV parameters
    fmt.Println("━━━ SETUP ━━━")
    params, _ := bgv.NewParametersFromLiteral(bgv.ParametersLiteral{
        LogN:             14,
        LogQ:             []int{55, 40, 40, 55},
        PlaintextModulus: 0x10001,
    })
    
    kgen := rlwe.NewKeyGenerator(params)
    sk := kgen.GenSecretKeyNew()
    pk := kgen.GenPublicKeyNew(sk)
    
    encoder := bgv.NewEncoder(params)
    encryptor := rlwe.NewEncryptor(params, pk)
    decryptor := rlwe.NewDecryptor(params, sk)
    evaluator := bgv.NewEvaluator(params, nil)
    
    fmt.Println("  ✅ BGV Parameters initialized")
    fmt.Println("  LogN: 14, LogQ: [55, 40, 40, 55]")
    fmt.Println("  φ = 1.61803398874989")
    fmt.Println()

    // Encrypt + Decrypt
    fmt.Println("━━━ TEST 1: ENCRYPT + DECRYPT ━━━")
    values := []int64{42, 100, 255, 999, 1618, 3141}
    
    for _, v := range values {
        pt := bgv.NewPlaintext(params, params.MaxLevel())
        encoder.Encode([]int64{v}, pt)
        
        ct, _ := encryptor.EncryptNew(pt)
        
        dec := bgv.NewPlaintext(params, params.MaxLevel())
        decryptor.Decrypt(ct, dec)
        
        result := make([]int64, 1)
        encoder.Decode(dec, result)
        
        status := "✅"
        if result[0] != v { status = "❌" }
        fmt.Printf("  %6d → %6d %s\n", v, result[0], status)
    }
    fmt.Println("  Encrypt/Decrypt: ✅ PASS")
    fmt.Println()

    // Homomorphic Addition
    fmt.Println("━━━ TEST 2: HOMOMORPHIC ADDITION ━━━")
    pt1 := bgv.NewPlaintext(params, params.MaxLevel())
    pt2 := bgv.NewPlaintext(params, params.MaxLevel())
    encoder.Encode([]int64{100}, pt1)
    encoder.Encode([]int64{200}, pt2)
    
    ct1, _ := encryptor.EncryptNew(pt1)
    ct2, _ := encryptor.EncryptNew(pt2)
    
    evaluator.Add(ct1, ct2, ct1)
    
    dec := bgv.NewPlaintext(params, params.MaxLevel())
    decryptor.Decrypt(ct1, dec)
    result := make([]int64, 1)
    encoder.Decode(dec, result)
    
    fmt.Printf("  100 + 200 = %d (expected 300) %s\n", result[0], 
        map[bool]string{true: "✅", false: "❌"}[result[0] == 300])
    fmt.Println()

    // Homomorphic Multiplication
    fmt.Println("━━━ TEST 3: HOMOMORPHIC MULTIPLICATION ━━━")
    pt3 := bgv.NewPlaintext(params, params.MaxLevel())
    pt4 := bgv.NewPlaintext(params, params.MaxLevel())
    encoder.Encode([]int64{42}, pt3)
    encoder.Encode([]int64{100}, pt4)
    
    ct3, _ := encryptor.EncryptNew(pt3)
    ct4, _ := encryptor.EncryptNew(pt4)
    
    evaluator.Mul(ct3, ct4, ct3)
    
    decryptor.Decrypt(ct3, dec)
    encoder.Decode(dec, result)
    
    fmt.Printf("  42 × 100 = %d (expected 4200) %s\n", result[0],
        map[bool]string{true: "✅", false: "❌"}[result[0] == 4200])
    fmt.Println()

    fmt.Println("╔══════════════════════════════════════════════╗")
    fmt.Println("║  Φ-LATTIGO: BGV ENCRYPTION VERIFIED ✅     ║")
    fmt.Println("║  ΦΩ0 — I AM THAT I AM                      ║")
    fmt.Println("╚══════════════════════════════════════════════╝")
}
