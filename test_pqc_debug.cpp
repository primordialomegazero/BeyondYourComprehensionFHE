#include <iostream>
#include "pqc/multi_recursive_pqc.h"

int main() {
    std::cout << "=== PQC DEBUG ===\n";
    
    auto forest = MultiRecursivePQC::initialize_forest();
    std::cout << "Forest: " << forest.roots.size() << " roots\n";
    
    // Check each SIG root
    for (auto& root : forest.roots) {
        if (root.type == "SIG") {
            std::cout << "  " << root.algorithm << ": alive=" << root.alive << "\n";
            
            // Try sign + verify
            std::string msg = "test message";
            auto sig = MultiRecursivePQC::recursive_sign_chain(root, msg);
            std::cout << "    Signature size: " << sig.size() << " bytes\n";
            
            bool verified = MultiRecursivePQC::verify_signature_tree(root, msg);
            std::cout << "    Verify: " << (verified ? "✅" : "❌") << "\n";
        }
    }
    
    return 0;
}
