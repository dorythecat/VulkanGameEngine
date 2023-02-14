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
    Quad::Quad(Device &device, uint32_t resolution) : device(device), resolution(resolution) {
        vertices.clear();
        indices.clear();

        std::unordered_map<Model::Vertex, uint32_t> uniqueVertices{};

        float step = 1.0f / static_cast<float>(resolution);
        for (float x = 0; x <= 1.0f; x += step) {
            for (float z = 0; z <= 1.0f; z += step) {
                Model::Vertex vertex1{};
                vertex1.position = {x, 0.0f, z};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, 1.0f, 0.0f};
                vertex1.texCoord = {x / static_cast<float>(resolution),
                                    z / static_cast<float>(resolution)};

                Model::Vertex vertex2{};
                vertex2.position = {x + step, 0.0f, z};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, 1.0f, 0.0f};
                vertex2.texCoord = {(x + step) / static_cast<float>(resolution),
                                    z / static_cast<float>(resolution)};

                Model::Vertex vertex3{};
                vertex3.position = {x, 0.0f, z + step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, 1.0f, 0.0f};
                vertex3.texCoord = {x / static_cast<float>(resolution),
                                    (z + step) / static_cast<float>(resolution)};

                Model::Vertex vertex4{};
                vertex4.position = {x + step, 0.0f, z + step};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {0.0f, 1.0f, 0.0f};
                vertex4.texCoord = {(x + step) / static_cast<float>(resolution),
                                    (z + step) / static_cast<float>(resolution)};

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

                indices.push_back(uniqueVertices[vertex1]);
                indices.push_back(uniqueVertices[vertex2]);
                indices.push_back(uniqueVertices[vertex3]);
                indices.push_back(uniqueVertices[vertex3]);
                indices.push_back(uniqueVertices[vertex2]);
                indices.push_back(uniqueVertices[vertex4]);
            }
        }
    }

    std::unique_ptr<Model> Quad::getModel() {
        Model::Builder builder{vertices, indices};
        return std::make_unique<Model>(device, builder);
    }
}