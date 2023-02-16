#ifndef QUAD_HPP
#define QUAD_HPP

#include <cstdint>
#include <vector>

#include "../../model/model.hpp"
#include "../procedural_mesh.hpp"

namespace Engine::Procedural {
    class Quad : public ProceduralMesh {
    public:
        Quad(Device &device, uint32_t resolution) : ProceduralMesh(device, resolution) {}

        void generateModel() override;
    };
}

#endif