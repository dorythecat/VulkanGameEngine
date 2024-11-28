#ifndef CUBE_HPP
#define CUBE_HPP

#include <cstdint>
#include <vector>

#include "../../model/model.hpp"
#include "../procedural_mesh.hpp"

namespace Engine::Procedural {
    class Cube final : public ProceduralMesh {
    public:
        Cube(Device &device, const uint32_t resolution) : ProceduralMesh(device, resolution) {}

        void generateModel() override;
    };
}

#endif