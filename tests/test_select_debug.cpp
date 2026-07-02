#include <iostream>
#include <cstdint>

// Copy the exact select_engine function
uint64_t nonce_ = 0x9E3779B97F4A7C15ULL;
constexpr int TOTAL_ENGINES = 9;

int select_engine(uint64_t op_id, int pass) {
    uint64_t h = nonce_ ^ op_id ^ (pass * 0x9E3779B97F4A7C15ULL);
    h ^= h >> 33;
    h *= 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    h ^= (h << 13);
    h *= 0x9E3779B97F4A7C15ULL;
    return (int)(h % TOTAL_ENGINES);
}

int main() {
    const char* names[] = {"Golden","Riemann","Fibonacci","Gödel","Cantor","Turing","Heisenberg","Nietzsche","Schrödinger"};
    
    std::cout << "Testing select_engine with op_id=0..9, pass=0..2:\n\n";
    
    for(int op=0; op<10; op++) {
        std::cout << "Op " << op << ": ";
        for(int pass=0; pass<3; pass++) {
            int eng = select_engine(op, pass);
            std::cout << names[eng] << (pass<2 ? " + " : "");
        }
        std::cout << "\n";
    }
    
    return 0;
}
