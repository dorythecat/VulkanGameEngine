#ifndef CUBE_HPP
#define CUBE_HPP

#include <cstdint>
#include <vector>

#include "../../model/model.hpp"
#include "../procedural_mesh.hpp"

namespace Engine::Procedural {
    class Cube : public ProceduralMesh {
    public:
        Cube(Device &device, uint32_t resolution) : ProceduralMesh(device, resolution) {}

        void generateModel() override;
    };
}

#endif