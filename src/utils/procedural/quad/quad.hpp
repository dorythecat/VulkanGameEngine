#ifndef QUAD_HPP
#define QUAD_HPP

#include <cstdint>
#include <vector>

#include "../../model/model.hpp"

namespace Engine::Procedural {
    class Quad {
    public:
        Quad(Device &device, uint32_t resolution);
        ~Quad() = default;

        Quad(const Quad&) = delete;
        Quad& operator=(const Quad&) = delete;

        std::unique_ptr<Model> getModel();
    private:
        Device &device;

        uint32_t resolution;

        std::vector<Model::Vertex> vertices{};
        std::vector<uint32_t> indices{};
    };
}

#endif