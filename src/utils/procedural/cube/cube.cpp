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
    Cube::Cube(Device &device, uint32_t resolution) : device(device), resolution(resolution) {
        vertices.clear();
        indices.clear();

        std::unordered_map<Model::Vertex, uint32_t> uniqueVertices{};

        // TODO(Dory): I'm sure this could be done in a better way, for now let's roll with this.
        // For example, Up and Down faces are the same, but with different Y values. Same for Left and Right and Front and Back.
        float step = 1.0f / static_cast<float>(resolution);
        for (float x = 0.0f; x < 1.0f - step; x += step) {
            // Front face
            for (float y = 0.0f; y < 1.0f - step; y += step) {
                Model::Vertex vertex1{};
                vertex1.position = {x, y, 0.0f};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, 0.0f, -1.0f};
                vertex1.texCoord = {x / static_cast<float>(resolution),
                                    y / static_cast<float>(resolution)};

                Model::Vertex vertex2{};
                vertex2.position = {x + step, y, 0.0f};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, 0.0f, -1.0f};
                vertex2.texCoord = {(x + step) / static_cast<float>(resolution),
                                    y / static_cast<float>(resolution)};

                Model::Vertex vertex3{};
                vertex3.position = {x, y + step, 0.0f};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, 0.0f, -1.0f};
                vertex3.texCoord = {x / static_cast<float>(resolution),
                                    (y + step) / static_cast<float>(resolution)};

                Model::Vertex vertex4{};
                vertex4.position = {x + step, y + step, 0.0f};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {0.0f, 0.0f, -1.0f};
                vertex4.texCoord = {(x + step) / static_cast<float>(resolution),
                                    (y + step) / static_cast<float>(resolution)};

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
            for (float y = 0.0f; y < 1.0f - step; y += step) {
                Model::Vertex vertex1{};
                vertex1.position = {x, y, 1.0f};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, 0.0f, 1.0f};
                vertex1.texCoord = {x / static_cast<float>(resolution),
                                    y / static_cast<float>(resolution)};

                Model::Vertex vertex2{};
                vertex2.position = {x + step, y, 1.0f};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, 0.0f, 1.0f};
                vertex2.texCoord = {(x + step) / static_cast<float>(resolution),
                                    y / static_cast<float>(resolution)};

                Model::Vertex vertex3{};
                vertex3.position = {x, y + step, 1.0f};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, 0.0f, 1.0f};
                vertex3.texCoord = {x / static_cast<float>(resolution),
                                    (y + step) / static_cast<float>(resolution)};

                Model::Vertex vertex4{};
                vertex4.position = {x + step, y + step, 1.0f};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {0.0f, 0.0f, 1.0f};
                vertex4.texCoord = {(x + step) / static_cast<float>(resolution),
                                    (y + step) / static_cast<float>(resolution)};

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
            for (float z = 0.0f; z < 1.0f - step; z += step) {
                Model::Vertex vertex1{};
                vertex1.position = {x, 0.0f, z};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, -1.0f, 0.0f};
                vertex1.texCoord = {x / static_cast<float>(resolution),
                                    z / static_cast<float>(resolution)};

                Model::Vertex vertex2{};
                vertex2.position = {x + step, 0.0f, z};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, -1.0f, 0.0f};
                vertex2.texCoord = {(x + step) / static_cast<float>(resolution),
                                    z / static_cast<float>(resolution)};

                Model::Vertex vertex3{};
                vertex3.position = {x, 0.0f, z + step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, -1.0f, 0.0f};
                vertex3.texCoord = {x / static_cast<float>(resolution),
                                    (z + step) / static_cast<float>(resolution)};

                Model::Vertex vertex4{};
                vertex4.position = {x + step, 0.0f, z + step};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {0.0f, -1.0f, 0.0f};
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
            for (float z = 0.0f; z < 1.0f - step; z += step) {
                Model::Vertex vertex1{};
                vertex1.position = {x, 1.0f, z};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {0.0f, 1.0f, 0.0f};
                vertex1.texCoord = {x / static_cast<float>(resolution),
                                    z / static_cast<float>(resolution)};

                Model::Vertex vertex2{};
                vertex2.position = {x + step, 1.0f, z};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {0.0f, 1.0f, 0.0f};
                vertex2.texCoord = {(x + step) / static_cast<float>(resolution),
                                    z / static_cast<float>(resolution)};

                Model::Vertex vertex3{};
                vertex3.position = {x, 1.0f, z + step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {0.0f, 1.0f, 0.0f};
                vertex3.texCoord = {x / static_cast<float>(resolution),
                                    (z + step) / static_cast<float>(resolution)};

                Model::Vertex vertex4{};
                vertex4.position = {x + step, 1.0f, z + step};
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

        for(float y = 0.0f; y < 1.0f - step; y += step) {
            // Left face
            for(float z = 0.0f; z < 1.0f - step; z += step) {
                Model::Vertex vertex1{};
                vertex1.position = {0.0f, y, z};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {1.0f, 0.0f, 0.0f};
                vertex1.texCoord = {y / static_cast<float>(resolution),
                                    z / static_cast<float>(resolution)};

                Model::Vertex vertex2{};
                vertex2.position = {0.0f, y + step, z};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {1.0f, 0.0f, 0.0f};
                vertex2.texCoord = {(y + step) / static_cast<float>(resolution),
                                    z / static_cast<float>(resolution)};

                Model::Vertex vertex3{};
                vertex3.position = {0.0f, y, z + step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {1.0f, 0.0f, 0.0f};
                vertex3.texCoord = {y / static_cast<float>(resolution),
                                    (z + step) / static_cast<float>(resolution)};

                Model::Vertex vertex4{};
                vertex4.position = {0.0f, y + step, z + step};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {1.0f, 0.0f, 0.0f};
                vertex4.texCoord = {(y + step) / static_cast<float>(resolution),
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
            for (float z = 0; z < 1.0f - step; z += step) {
                Model::Vertex vertex1{};
                vertex1.position = {1.0f, y, z};
                vertex1.color = {1.0f, 1.0f, 1.0f};
                vertex1.normal = {-1.0f, 0.0f, 0.0f};
                vertex1.texCoord = {y / static_cast<float>(resolution),
                                    z / static_cast<float>(resolution)};

                Model::Vertex vertex2{};
                vertex2.position = {1.0f, y + step, z};
                vertex2.color = {1.0f, 1.0f, 1.0f};
                vertex2.normal = {-1.0f, 0.0f, 0.0f};
                vertex2.texCoord = {(y + step) / static_cast<float>(resolution),
                                    z / static_cast<float>(resolution)};

                Model::Vertex vertex3{};
                vertex3.position = {1.0f, y, z + step};
                vertex3.color = {1.0f, 1.0f, 1.0f};
                vertex3.normal = {-1.0f, 0.0f, 0.0f};
                vertex3.texCoord = {y / static_cast<float>(resolution),
                                    (z + step) / static_cast<float>(resolution)};

                Model::Vertex vertex4{};
                vertex4.position = {1.0f, y + step, z + step};
                vertex4.color = {1.0f, 1.0f, 1.0f};
                vertex4.normal = {-1.0f, 0.0f, 0.0f};
                vertex4.texCoord = {(y + step) / static_cast<float>(resolution),
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

    std::unique_ptr<Model> Cube::getModel() {
        Model::Builder builder{vertices, indices};
        return std::make_unique<Model>(device, builder);
    }
}