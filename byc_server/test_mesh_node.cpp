#include <iostream>
#include "phi_mesh_node.h"

int main() {
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  Φ-MESH NODE — OPTIMIZED TEST               ║" << std::endl;
    std::cout << "║  Zero-config. Self-discovering.              ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;
    
    phi_mesh::PhiMeshNode node("node-manila-phi");
    
    // Add some peers (simulating discovered nodes)
    node.add_peer("node-tokyo", "192.168.1.2:8080", 12.50);
    node.add_peer("node-sydney", "192.168.1.3:8080", 12.32);
    node.add_peer("node-london", "192.168.1.4:8080", 12.08);
    node.add_peer("node-nyc", "192.168.1.5:8080", 11.94);
    
    node.start();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    node.print_status();
    node.stop();
    
    return 0;
}
