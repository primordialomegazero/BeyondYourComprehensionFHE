#include <iostream>
#include "zkp/multi_recursive_zkp.h"

int main() {
    std::cout << "=== ZKP DEBUG ===\n";
    
    // Test single proof
    auto single = MultiRecursiveZKP::prove_single("test", 0, 0);
    bool single_ok = MultiRecursiveZKP::verify_single(single);
    std::cout << "Single proof: " << (single_ok ? "✅" : "❌") << "\n";
    
    // Test recursive tree
    auto tree = MultiRecursiveZKP::prove_recursive("test", 0, 3, 2);
    std::cout << "Tree: " << tree.total_proofs << " proofs, depth " << tree.max_depth << "\n";
    std::cout << "Root children: " << tree.root.children.size() << "\n";
    
    // Verify each level
    bool root_ok = MultiRecursiveZKP::verify_single(tree.root);
    std::cout << "Root verify: " << (root_ok ? "✅" : "❌") << "\n";
    
    bool tree_ok = MultiRecursiveZKP::verify_tree(tree);
    std::cout << "Tree verify: " << (tree_ok ? "✅" : "❌") << "\n";
    
    return tree_ok ? 0 : 1;
}
