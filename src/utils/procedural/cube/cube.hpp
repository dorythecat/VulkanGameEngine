#ifndef CUBE_HPP
#define CUBE_HPP

#include <cstdint>
#include <vector>

#include "../../model/model.hpp"

namespace Engine::Procedural {
    class Cube {
    public:
        Cube(Device &device, uint32_t resolution);
        ~Cube() = default;

        Cube(const Cube&) = delete;
        Cube& operator=(const Cube&) = delete;

        std::unique_ptr<Model> getModel();
    private:
        Device &device;

        uint32_t resolution;

        std::vector<Model::Vertex> vertices{};
        std::vector<uint32_t> indices{};
    };
}

#endif