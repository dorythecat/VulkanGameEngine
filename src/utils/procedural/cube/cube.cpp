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
        float stepSquared = step * step;

        // TODO(Dory): I'm sure this could be done in a better way, for now let's roll with this.
        // For example, Up and Down faces are the same, but with different Y values. Same for Left and Right and Front and Back.
        // Also, I don't like using for loops with floats, but I don't know how to do it in a better way, at least for now.
        for (float x = 0.0f; x < 1.0f; x += step) {
            float xStep = x * step;

            // Front face
            for (float y = 0.0f; y < 1.0f; y += step) {
                float yStep = y * step;

                Model::Vertex vertex1{};
                vertex1.position = {x, y, 0.0f};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, 0.0f, -1.0f};
                vertex1.texCoord = {xStep,
                                    yStep};

                Model::Vertex vertex2{};
                vertex2.position = {x + step, y, 0.0f};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, 0.0f, -1.0f};
                vertex2.texCoord = {xStep + stepSquared,
                                    yStep};

                Model::Vertex vertex3{};
                vertex3.position = {x, y + step, 0.0f};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, 0.0f, -1.0f};
                vertex3.texCoord = {xStep,
                                    yStep + stepSquared};

                Model::Vertex vertex4{};
                vertex4.position = {x + step, y + step, 0.0f};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {0.0f, 0.0f, -1.0f};
                vertex4.texCoord = {xStep + stepSquared,
                                    yStep + stepSquared};

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
            for (float y = 0.0f; y < 1.0f; y += step) {
                float yStep = y * step;

                Model::Vertex vertex1{};
                vertex1.position = {x, y, 1.0f};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, 0.0f, 1.0f};
                vertex1.texCoord = {xStep,
                                    yStep};

                Model::Vertex vertex2{};
                vertex2.position = {x + step, y, 1.0f};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, 0.0f, 1.0f};
                vertex2.texCoord = {xStep + stepSquared,
                                    yStep};

                Model::Vertex vertex3{};
                vertex3.position = {x, y + step, 1.0f};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, 0.0f, 1.0f};
                vertex3.texCoord = {xStep,
                                    yStep + stepSquared};

                Model::Vertex vertex4{};
                vertex4.position = {x + step, y + step, 1.0f};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {0.0f, 0.0f, 1.0f};
                vertex4.texCoord = {xStep + stepSquared,
                                    yStep + stepSquared};

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
            for (float z = 0.0f; z < 1.0f; z += step) {
                float zStep = z * step;

                Model::Vertex vertex1{};
                vertex1.position = {x, 0.0f, z};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, -1.0f, 0.0f};
                vertex1.texCoord = {xStep,
                                    z / static_cast<float>(resolution)};

                Model::Vertex vertex2{};
                vertex2.position = {x + step, 0.0f, z};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, -1.0f, 0.0f};
                vertex2.texCoord = {xStep + stepSquared,
                                    zStep};

                Model::Vertex vertex3{};
                vertex3.position = {x, 0.0f, z + step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, -1.0f, 0.0f};
                vertex3.texCoord = {xStep,
                                    zStep + stepSquared};

                Model::Vertex vertex4{};
                vertex4.position = {x + step, 0.0f, z + step};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {0.0f, -1.0f, 0.0f};
                vertex4.texCoord = {xStep + stepSquared,
                                    zStep + stepSquared};

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
            for (float z = 0.0f; z < 1.0f; z += step) {
                float zStep = z * step;

                Model::Vertex vertex1{};
                vertex1.position = {x, 1.0f, z};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, 1.0f, 0.0f};
                vertex1.texCoord = {xStep,
                                    zStep};

                Model::Vertex vertex2{};
                vertex2.position = {x + step, 1.0f, z};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, 1.0f, 0.0f};
                vertex2.texCoord = {xStep + stepSquared,
                                    zStep};

                Model::Vertex vertex3{};
                vertex3.position = {x, 1.0f, z + step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, 1.0f, 0.0f};
                vertex3.texCoord = {xStep,
                                    zStep + stepSquared};

                Model::Vertex vertex4{};
                vertex4.position = {x + step, 1.0f, z + step};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {0.0f, 1.0f, 0.0f};
                vertex4.texCoord = {xStep + stepSquared,
                                    zStep + stepSquared};

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

        for(float y = 0.0f; y < 1.0f; y += step) {
            float yStep = y * step;

            // Left face
            for(float z = 0.0f; z < 1.0f; z += step) {
                float zStep = z * step;

                Model::Vertex vertex1{};
                vertex1.position = {0.0f, y, z};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {1.0f, 0.0f, 0.0f};
                vertex1.texCoord = {yStep,
                                    zStep};

                Model::Vertex vertex2{};
                vertex2.position = {0.0f, y + step, z};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {1.0f, 0.0f, 0.0f};
                vertex2.texCoord = {yStep + stepSquared,
                                    zStep};

                Model::Vertex vertex3{};
                vertex3.position = {0.0f, y, z + step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {1.0f, 0.0f, 0.0f};
                vertex3.texCoord = {yStep,
                                    zStep + stepSquared};

                Model::Vertex vertex4{};
                vertex4.position = {0.0f, y + step, z + step};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {1.0f, 0.0f, 0.0f};
                vertex4.texCoord = {yStep + stepSquared,
                                    zStep + stepSquared};

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
            for (float z = 0; z < 1.0f; z += step) {
                float zStep = z * step;

                Model::Vertex vertex1{};
                vertex1.position = {1.0f, y, z};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {-1.0f, 0.0f, 0.0f};
                vertex1.texCoord = {yStep,
                                    zStep};

                Model::Vertex vertex2{};
                vertex2.position = {1.0f, y + step, z};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {-1.0f, 0.0f, 0.0f};
                vertex2.texCoord = {yStep + stepSquared,
                                    zStep};

                Model::Vertex vertex3{};
                vertex3.position = {1.0f, y, z + step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {-1.0f, 0.0f, 0.0f};
                vertex3.texCoord = {yStep,
                                    zStep + stepSquared};

                Model::Vertex vertex4{};
                vertex4.position = {1.0f, y + step, z + step};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {-1.0f, 0.0f, 0.0f};
                vertex4.texCoord = {yStep + stepSquared,
                                    zStep + stepSquared};

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