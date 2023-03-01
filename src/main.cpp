#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "application.hpp"

// TODO(Dory): Change to a proper naming and file structure convention.
int main() {
    Engine::Application app {};

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } return EXIT_SUCCESS;
}