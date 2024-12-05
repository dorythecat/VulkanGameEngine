#include "terrain.hpp"

template<>
struct std::hash<Engine::Model::Vertex> {
    size_t operator()(Engine::Model::Vertex const &vertex) const noexcept {
        size_t seed = 0;
        hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.texCoord);
        return seed;
    }
};

namespace Engine::Procedural {
    void Terrain::generateModel() {
        // Reserve space for the vertices and indices. This is done to avoid reallocations, hence more performance.
        uint32_t reserveSpace = 4 * resolution * resolution;
        assert(values.size() == resolution * resolution && "Cannot generate terrain with an incomplete value list!");
        vertices.reserve(reserveSpace);
        indices.reserve(6 * reserveSpace);
        std::unordered_map<Model::Vertex, uint32_t> uniqueVertices(reserveSpace);

        // Precomputed values for the loops.
        float step = 1.0f / static_cast<float>(resolution);

        for (uint32_t x = 0; x < resolution; x++) {
            float xStep = static_cast<float>(x) * step;
            float x1Step = xStep + step;

            for (uint32_t z = 0; z < resolution; z++) {
                float zStep = static_cast<float>(z) * step;
                float z1Step = zStep + step;

                Model::Vertex vertex1{};
                vertex1.position = {xStep, values[z * resolution + x], zStep};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, -1.0f, 0.0f};
                vertex1.texCoord = {xStep,
                                    zStep};

                Model::Vertex vertex2{};
                vertex2.position = {x1Step, values[z * resolution + x], zStep};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, -1.0f, 0.0f};
                vertex2.texCoord = {x1Step,
                                    zStep};

                Model::Vertex vertex3{};
                vertex3.position = {xStep, values[z * resolution + x], z1Step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, -1.0f, 0.0f};
                vertex3.texCoord = {xStep,
                                    z1Step};

                Model::Vertex vertex4{};
                vertex4.position = {x1Step, values[z * resolution + x], z1Step};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {0.0f, -1.0f, 0.0f};
                vertex4.texCoord = {x1Step,
                                    z1Step};

                if (!uniqueVertices.contains(vertex1)) {
                    uniqueVertices[vertex1] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex1);
                }

                if (!uniqueVertices.contains(vertex2)) {
                    uniqueVertices[vertex2] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex2);
                }

                if (!uniqueVertices.contains(vertex3)) {
                    uniqueVertices[vertex3] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex3);
                }

                if (!uniqueVertices.contains(vertex4)) {
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

                // Add interconnecting quads
                // (technically disconnected from the main quads, but they're so close they count as connected idc)
                //TODO(Dory): Add proper normals to the interconnecting quads

                // x axis
                if (x != 0 && values[z * resolution + x] != values[z * resolution + x - 1]) {
                    vertex1.position = {xStep, values[z * resolution + x - 1], zStep};
                    // vertex1.normal = {0.0f, -1.0f, 0.0f};
                    vertex1.texCoord = {xStep,
                                        zStep};

                    vertex2.position = {xStep, values[z * resolution + x], zStep};
                    // vertex2.normal = {0.0f, -1.0f, 0.0f};
                    vertex2.texCoord = {xStep,
                                        zStep};

                    vertex3.position = {xStep, values[z * resolution + x - 1], z1Step};
                    // vertex3.normal = {0.0f, -1.0f, 0.0f};
                    vertex3.texCoord = {xStep,
                                        z1Step};

                    vertex4.position = {xStep, values[z * resolution + x], z1Step};
                    // vertex4.normal = {0.0f, -1.0f, 0.0f};
                    vertex4.texCoord = {xStep,
                                        z1Step};

                    if (!uniqueVertices.contains(vertex1)) {
                        uniqueVertices[vertex1] = static_cast<uint32_t>(vertices.size());
                        vertices.push_back(vertex1);
                    }

                    if (!uniqueVertices.contains(vertex2)) {
                        uniqueVertices[vertex2] = static_cast<uint32_t>(vertices.size());
                        vertices.push_back(vertex2);
                    }

                    if (!uniqueVertices.contains(vertex3)) {
                        uniqueVertices[vertex3] = static_cast<uint32_t>(vertices.size());
                        vertices.push_back(vertex3);
                    }

                    if (!uniqueVertices.contains(vertex4)) {
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

                // z axis
                if (z != 0 && values[z * resolution + x] != values[(z - 1) * resolution + x ]) {
                    vertex1.position = {xStep, values[(z - 1) * resolution + x], zStep};
                    // vertex1.normal = {0.0f, -1.0f, 0.0f};
                    vertex1.texCoord = {xStep,
                                        zStep};

                    vertex2.position = {x1Step, values[(z - 1) * resolution + x], zStep};
                    // vertex2.normal = {0.0f, -1.0f, 0.0f};
                    vertex2.texCoord = {x1Step,
                                        zStep};

                    vertex3.position = {xStep, values[z * resolution + x], zStep};
                    // vertex3.normal = {0.0f, -1.0f, 0.0f};
                    vertex3.texCoord = {xStep,
                                        zStep};

                    vertex4.position = {x1Step, values[z * resolution + x], zStep};
                    // vertex4.normal = {0.0f, -1.0f, 0.0f};
                    vertex4.texCoord = {x1Step,
                                        zStep};

                    if (!uniqueVertices.contains(vertex1)) {
                        uniqueVertices[vertex1] = static_cast<uint32_t>(vertices.size());
                        vertices.push_back(vertex1);
                    }

                    if (!uniqueVertices.contains(vertex2)) {
                        uniqueVertices[vertex2] = static_cast<uint32_t>(vertices.size());
                        vertices.push_back(vertex2);
                    }

                    if (!uniqueVertices.contains(vertex3)) {
                        uniqueVertices[vertex3] = static_cast<uint32_t>(vertices.size());
                        vertices.push_back(vertex3);
                    }

                    if (!uniqueVertices.contains(vertex4)) {
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
        }

        builder = Model::Builder(vertices, indices);
    }
}