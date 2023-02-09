#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <cstdint>
#include <vector>

#include "../../model/model.hpp"

namespace Engine::Procedural {
    class Terrain {
    public:
        Terrain(Device &device, uint32_t resolution);
        ~Terrain() = default;

        Terrain(const Terrain&) = delete;
        Terrain& operator=(const Terrain&) = delete;

        std::unique_ptr<Model> getModel();
    private:
        Device &device;

        uint32_t resolution;

        std::vector<Model::Vertex> vertices{};
        std::vector<uint32_t> indices{};
    };
}

#endif