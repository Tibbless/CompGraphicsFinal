#include "glfw/glfw_app.h"
#include <iostream>

int main() {
    std::cout << "=== Eerie City ===" << std::endl;
    std::cout << "Low-res 3D cityscape renderer" << std::endl;
    std::cout << std::endl;
    
    GLFWApp app(800, 600, "Eerie City - PS1 Style");
    
    if (!app.initialize()) {
        std::cerr << "Failed to initialize application" << std::endl;
        return -1;
    }
    
    app.run();
    
    return 0;
}