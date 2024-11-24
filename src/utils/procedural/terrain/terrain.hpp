#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <cstdint>
#include <utility>
#include <vector>

#include "../../model/model.hpp"
#include "../procedural_mesh.hpp"

namespace Engine::Procedural {
    class Terrain : public ProceduralMesh {
    public:
        Terrain(Device &device, uint32_t resolution, const std::vector<float> values) :
            ProceduralMesh(device, resolution), values(values) {}

        void generateModel() override;

    private:
        std::vector<float> values;
    };
}

#endif