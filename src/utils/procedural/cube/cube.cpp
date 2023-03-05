#include "cube.hpp"

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
    void Cube::generateModel() {
        std::unordered_map<Model::Vertex, uint32_t> uniqueVertices{};

        // Reserve space for the vertices and indices. This is done to avoid reallocations, hence more performance.
        uint32_t reserveSpace = 6 * resolution * resolution;
        vertices.reserve(reserveSpace);
        indices.reserve(reserveSpace);
        uniqueVertices.reserve(reserveSpace);

        // Precomputed values for the loops.
        float step = 1.0f / static_cast<float>(resolution);

        // TODO(Dory): I'm sure this could be done in a better way, for now let's roll with this.
        // For example, Up and Down faces are the same, but with different Y values. Same for Left and Right and Front and Back.
        // Also, I don't like using for loops with floats, but I don't know how to do it in a better way, at least for now.
        for (uint32_t x = 0; x < resolution; x++) {
            float xStep = static_cast<float>(x) * step;
            float x1Step = xStep + step;

            // Front face
            for (uint32_t y = 0; y < resolution; y++) {
                float yStep = static_cast<float>(y) * step;
                float y1Step = yStep + step;

                Model::Vertex vertex1{};
                vertex1.position = {xStep, -yStep, 0.0f};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, 0.0f, -1.0f};
                vertex1.texCoord = {xStep,
                                    yStep};

                Model::Vertex vertex2{};
                vertex2.position = {x1Step, -yStep, 0.0f};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, 0.0f, -1.0f};
                vertex2.texCoord = {x1Step,
                                    yStep};

                Model::Vertex vertex3{};
                vertex3.position = {xStep, -y1Step, 0.0f};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, 0.0f, -1.0f};
                vertex3.texCoord = {xStep,
                                    y1Step};

                Model::Vertex vertex4{};
                vertex4.position = {x1Step, -y1Step, 0.0f};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {0.0f, 0.0f, -1.0f};
                vertex4.texCoord = {x1Step,
                                    y1Step};

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
                indices.push_back(uniqueVertices[vertex2]);
                indices.push_back(uniqueVertices[vertex4]);
                indices.push_back(uniqueVertices[vertex3]);
            }

            // Back face
            for (uint32_t y = 0; y < resolution; y++) {
                float yStep = static_cast<float>(y) * step;
                float y1Step = yStep + step;

                Model::Vertex vertex1{};
                vertex1.position = {xStep, -yStep, 1.0f};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, 0.0f, 1.0f};
                vertex1.texCoord = {xStep,
                                    yStep};

                Model::Vertex vertex2{};
                vertex2.position = {x1Step, -yStep, 1.0f};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, 0.0f, 1.0f};
                vertex2.texCoord = {x1Step,
                                    yStep};

                Model::Vertex vertex3{};
                vertex3.position = {xStep, -y1Step, 1.0f};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, 0.0f, 1.0f};
                vertex3.texCoord = {xStep,
                                    y1Step};

                Model::Vertex vertex4{};
                vertex4.position = {x1Step, -y1Step, 1.0f};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {0.0f, 0.0f, 1.0f};
                vertex4.texCoord = {x1Step,
                                    y1Step};

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
                indices.push_back(uniqueVertices[vertex2]);
                indices.push_back(uniqueVertices[vertex4]);
                indices.push_back(uniqueVertices[vertex3]);
            }

            // Up face
            for (uint32_t z = 0; z < resolution; z++) {
                float zStep = static_cast<float>(z) * step;
                float z1Step = zStep + step;

                Model::Vertex vertex1{};
                vertex1.position = {xStep, 0.0f, zStep};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, -1.0f, 0.0f};
                vertex1.texCoord = {xStep,
                                    zStep};

                Model::Vertex vertex2{};
                vertex2.position = {x1Step, 0.0f, zStep};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, -1.0f, 0.0f};
                vertex2.texCoord = {x1Step,
                                    zStep};

                Model::Vertex vertex3{};
                vertex3.position = {xStep, 0.0f, z1Step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, -1.0f, 0.0f};
                vertex3.texCoord = {xStep,
                                    z1Step};

                Model::Vertex vertex4{};
                vertex4.position = {x1Step, 0.0f, z1Step};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {0.0f, -1.0f, 0.0f};
                vertex4.texCoord = {x1Step,
                                    z1Step};

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

            // Down face
            for (uint32_t z = 0; z < resolution; z++) {
                float zStep = static_cast<float>(z) * step;
                float z1Step = zStep + step;

                Model::Vertex vertex1{};
                vertex1.position = {xStep, -1.0f, zStep};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, 1.0f, 0.0f};
                vertex1.texCoord = {xStep,
                                    zStep};

                Model::Vertex vertex2{};
                vertex2.position = {x1Step, -1.0f, zStep};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, 1.0f, 0.0f};
                vertex2.texCoord = {x1Step,
                                    zStep};

                Model::Vertex vertex3{};
                vertex3.position = {xStep, -1.0f, z1Step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, 1.0f, 0.0f};
                vertex3.texCoord = {xStep,
                                    z1Step};

                Model::Vertex vertex4{};
                vertex4.position = {x1Step, -1.0f, z1Step};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {0.0f, 1.0f, 0.0f};
                vertex4.texCoord = {x1Step,
                                    z1Step};

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

        for(uint32_t y = 0; y < resolution; y++) {
            float yStep = static_cast<float>(y) * step;
            float y1Step = yStep + step;

            // Left face
            for(uint32_t z = 0; z < resolution; z++) {
                float zStep = static_cast<float>(z) * step;
                float z1Step = zStep + step;

                Model::Vertex vertex1{};
                vertex1.position = {0.0f, -yStep, zStep};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {1.0f, 0.0f, 0.0f};
                vertex1.texCoord = {yStep,
                                    zStep};

                Model::Vertex vertex2{};
                vertex2.position = {0.0f, -y1Step, zStep};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {1.0f, 0.0f, 0.0f};
                vertex2.texCoord = {y1Step,
                                    zStep};

                Model::Vertex vertex3{};
                vertex3.position = {0.0f, -yStep, z1Step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {1.0f, 0.0f, 0.0f};
                vertex3.texCoord = {yStep,
                                    z1Step};

                Model::Vertex vertex4{};
                vertex4.position = {0.0f, -y1Step, z1Step};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {1.0f, 0.0f, 0.0f};
                vertex4.texCoord = {y1Step,
                                    z1Step};

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


            // Right face
            for (uint32_t z = 0; z < resolution; z++) {
                float zStep = static_cast<float>(z) * step;
                float z1Step = zStep + step;

                Model::Vertex vertex1{};
                vertex1.position = {1.0f, -yStep, zStep};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {-1.0f, 0.0f, 0.0f};
                vertex1.texCoord = {yStep,
                                    zStep};

                Model::Vertex vertex2{};
                vertex2.position = {1.0f, -y1Step, zStep};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {-1.0f, 0.0f, 0.0f};
                vertex2.texCoord = {y1Step,
                                    zStep};

                Model::Vertex vertex3{};
                vertex3.position = {1.0f, -yStep, z1Step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {-1.0f, 0.0f, 0.0f};
                vertex3.texCoord = {yStep,
                                    z1Step};

                Model::Vertex vertex4{};
                vertex4.position = {1.0f, -y1Step, z1Step};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {-1.0f, 0.0f, 0.0f};
                vertex4.texCoord = {y1Step,
                                    z1Step};

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