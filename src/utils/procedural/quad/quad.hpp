#ifndef QUAD_HPP
#define QUAD_HPP

#include <cstdint>
#include <vector>

#include "../../model/model.hpp"
#include "../procedural_mesh.hpp"

namespace Engine::Procedural {
    class Quad final : public ProceduralMesh {
    public:
        Quad(Device &device, const uint32_t resolution) : ProceduralMesh(device, resolution) {}

        void generateModel() override;
    };
}

#endif