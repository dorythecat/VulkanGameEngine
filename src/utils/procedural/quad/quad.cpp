#include "quad.hpp"

namespace std {
    template<>
    struct hash<Engine::Model::Vertex> {
        size_t operator()(Engine::Model::Vertex const &vertex) const {
            size_t seed = 0;
            hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.texCoord);
            return seed;
        }
    };
}

namespace Engine::Procedural {
    void Quad::generateModel() {
        std::unordered_map<Model::Vertex, uint32_t> uniqueVertices{};

        // Reserve space for the vertices and indices. This is done to avoid reallocations, hence more performance.
        uint32_t reserveSpace = resolution * resolution;
        vertices.reserve(reserveSpace);
        indices.reserve(reserveSpace);
        uniqueVertices.reserve(reserveSpace);

        // Precomputed values for the loops.
        float step = 1.0f / static_cast<float>(resolution);
        float stepSquared = step * step;

        for (uint32_t x = 0; x < resolution; x++) {
            float xStep = static_cast<float>(x) * step;
            float xStepSquared = static_cast<float>(x) * stepSquared;

            for (uint32_t z = 0; z < resolution; z++) {
                float zStep = static_cast<float>(z) * step;
                float zStepSquared = static_cast<float>(z) * stepSquared;

                Model::Vertex vertex1{};
                vertex1.position = {xStep, 0.0f, zStep};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, 1.0f, 0.0f};
                vertex1.texCoord = {xStepSquared,
                                    zStepSquared};

                Model::Vertex vertex2{};
                vertex2.position = {xStep + step, 0.0f, zStep};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, 1.0f, 0.0f};
                vertex2.texCoord = {xStepSquared + stepSquared,
                                    zStepSquared};

                Model::Vertex vertex3{};
                vertex3.position = {xStep, 0.0f, zStep + step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, 1.0f, 0.0f};
                vertex3.texCoord = {xStepSquared,
                                    zStepSquared + stepSquared};

                Model::Vertex vertex4{};
                vertex4.position = {xStep + step, 0.0f, zStep + step};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {0.0f, 1.0f, 0.0f};
                vertex4.texCoord = {xStepSquared + stepSquared,
                                    zStepSquared + stepSquared};

                if (uniqueVertices.count(vertex1) == 0) {
                    uniqueVertices[vertex1] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex1);
                }

                if (uniqueVertices.count(vertex2) == 0) {
                    uniqueVertices[vertex2] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex2);
                }

                if (uniqueVertices.count(vertex3) == 0) {
                    uniqueVertices[vertex3] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex3);
                }

                if (uniqueVertices.count(vertex4) == 0) {
                    uniqueVertices[vertex4] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex4);
                }

                // Triangle 1
                indices.push_back(uniqueVertices[vertex1]);
                indices.push_back(uniqueVertices[vertex2]);
                indices.push_back(uniqueVertices[vertex3]);

                // Triangle 2
                indices.push_back(uniqueVertices[vertex3]);
                indices.push_back(uniqueVertices[vertex2]);
                indices.push_back(uniqueVertices[vertex4]);
            }
        }

        builder = *new Model::Builder{vertices, indices};
    }
}