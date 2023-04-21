#include "cube.hpp"

namespace std {
    template<>
    struct hash<Engine::Model::Vertex> {
        inline size_t operator()(Engine::Model::Vertex const &vertex) const {
            size_t seed = 0;
            hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.texCoord);
            return seed;
        }
    };
}

namespace Engine::Procedural {
    void Cube::generateModel() {
        std::unordered_map<Model::Vertex, uint32_t> uniqueVertices{};

        // Reserve space for the vertices and indices. This is done to avoid reallocations, which should give us better performance.
        uint32_t reserveSpace = 6 * resolution * resolution + 2;
        vertices.reserve(reserveSpace);
        indices.reserve(reserveSpace);
        uniqueVertices.reserve(reserveSpace);

        // Precomputed values for the loops.
        float step = 1.0f / static_cast<float>(resolution);

        for (uint32_t x = 0; x < resolution; x++) {
            float xStep = static_cast<float>(x) * step;
            float x1Step = xStep + step;

            // Front and back faces
            for (uint32_t y = 0; y < resolution; y++) {
                // Front face
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


                // Back face
                Model::Vertex vertex1p{};
                vertex1p.position = {xStep, -yStep, 1.0f};
                vertex1p.color = {1.0f, 1.0f, 1.0f};
                vertex1p.normal = {0.0f, 0.0f, 1.0f};
                vertex1p.texCoord = {xStep,
                                     yStep};

                Model::Vertex vertex2p{};
                vertex2p.position = {x1Step, -yStep, 1.0f};
                vertex2p.color = {1.0f, 1.0f, 1.0f};
                vertex2p.normal = {0.0f, 0.0f, 1.0f};
                vertex2p.texCoord = {x1Step,
                                     yStep};

                Model::Vertex vertex3p{};
                vertex3p.position = {xStep, -y1Step, 1.0f};
                vertex3p.color = {1.0f, 1.0f, 1.0f};
                vertex3p.normal = {0.0f, 0.0f, 1.0f};
                vertex3p.texCoord = {xStep,
                                     y1Step};

                Model::Vertex vertex4p{};
                vertex4p.position = {x1Step, -y1Step, 1.0f};
                vertex4p.color = {1.0f, 1.0f, 1.0f};
                vertex4p.normal = {0.0f, 0.0f, 1.0f};
                vertex4p.texCoord = {x1Step,
                                     y1Step};

                if (uniqueVertices.count(vertex1p) == 0) {
                    uniqueVertices[vertex1p] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex1p);
                }

                if (uniqueVertices.count(vertex2p) == 0) {
                    uniqueVertices[vertex2p] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex2p);
                }

                if (uniqueVertices.count(vertex3p) == 0) {
                    uniqueVertices[vertex3p] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex3p);
                }

                if (uniqueVertices.count(vertex4p) == 0) {
                    uniqueVertices[vertex4p] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex4p);
                }

                // Triangle 1
                indices.push_back(uniqueVertices[vertex1p]);
                indices.push_back(uniqueVertices[vertex3p]);
                indices.push_back(uniqueVertices[vertex2p]);

                // Triangle 2
                indices.push_back(uniqueVertices[vertex2p]);
                indices.push_back(uniqueVertices[vertex3p]);
                indices.push_back(uniqueVertices[vertex4p]);
            }

            // Top and bottom faces
            for (uint32_t z = 0; z < resolution; z++) {
                // Top face
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

                // Bottom face
                Model::Vertex vertex1p{};
                vertex1p.position = {xStep, 0.0f, zStep};
                vertex1p.color = {1.0f, 1.0f, 1.0f};
                vertex1p.normal = {0.0f, -1.0f, 0.0f};
                vertex1p.texCoord = {xStep,
                                     zStep};

                Model::Vertex vertex2p{};
                vertex2p.position = {x1Step, 0.0f, zStep};
                vertex2p.color = {1.0f, 1.0f, 1.0f};
                vertex2p.normal = {0.0f, -1.0f, 0.0f};
                vertex2p.texCoord = {x1Step,
                                     zStep};

                Model::Vertex vertex3p{};
                vertex3p.position = {xStep, 0.0f, z1Step};
                vertex3p.color = {1.0f, 1.0f, 1.0f};
                vertex3p.normal = {0.0f, -1.0f, 0.0f};
                vertex3p.texCoord = {xStep,
                                     z1Step};

                Model::Vertex vertex4p{};
                vertex4p.position = {x1Step, 0.0f, z1Step};
                vertex4p.color = {1.0f, 1.0f, 1.0f};
                vertex4p.normal = {0.0f, -1.0f, 0.0f};
                vertex4p.texCoord = {x1Step,
                                     z1Step};

                if (uniqueVertices.count(vertex1p) == 0) {
                    uniqueVertices[vertex1p] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex1p);
                }

                if (uniqueVertices.count(vertex2p) == 0) {
                    uniqueVertices[vertex2p] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex2p);
                }

                if (uniqueVertices.count(vertex3p) == 0) {
                    uniqueVertices[vertex3p] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex3p);
                }

                if (uniqueVertices.count(vertex4p) == 0) {
                    uniqueVertices[vertex4p] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex4p);
                }

                // Triangle 1
                indices.push_back(uniqueVertices[vertex3p]);
                indices.push_back(uniqueVertices[vertex2p]);
                indices.push_back(uniqueVertices[vertex1p]);

                // Triangle 2
                indices.push_back(uniqueVertices[vertex4p]);
                indices.push_back(uniqueVertices[vertex2p]);
                indices.push_back(uniqueVertices[vertex3p]);
            }
        }

        for(uint32_t y = 0; y < resolution; y++) {
            float yStep = static_cast<float>(y) * step;
            float y1Step = yStep + step;

            // Left and right faces
            for(uint32_t z = 0; z < resolution; z++) {
                // Left face
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

                // Right face
                Model::Vertex vertex1p{};
                vertex1p.position = {1.0f, -yStep, zStep};
                vertex1p.color = {1.0f, 1.0f, 1.0f};
                vertex1p.normal = {-1.0f, 0.0f, 0.0f};
                vertex1p.texCoord = {yStep,
                                     zStep};

                Model::Vertex vertex2p{};
                vertex2p.position = {1.0f, -y1Step, zStep};
                vertex2p.color = {1.0f, 1.0f, 1.0f};
                vertex2p.normal = {-1.0f, 0.0f, 0.0f};
                vertex2p.texCoord = {y1Step,
                                     zStep};

                Model::Vertex vertex3p{};
                vertex3p.position = {1.0f, -yStep, z1Step};
                vertex3p.color = {1.0f, 1.0f, 1.0f};
                vertex3p.normal = {-1.0f, 0.0f, 0.0f};
                vertex3p.texCoord = {yStep,
                                     z1Step};

                Model::Vertex vertex4p{};
                vertex4p.position = {1.0f, -y1Step, z1Step};
                vertex4p.color = {1.0f, 1.0f, 1.0f};
                vertex4p.normal = {-1.0f, 0.0f, 0.0f};
                vertex4p.texCoord = {y1Step,
                                     z1Step};

                if (uniqueVertices.count(vertex1p) == 0) {
                    uniqueVertices[vertex1p] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex1p);
                }

                if (uniqueVertices.count(vertex2p) == 0) {
                    uniqueVertices[vertex2p] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex2p);
                }

                if (uniqueVertices.count(vertex3p) == 0) {
                    uniqueVertices[vertex3p] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex3p);
                }

                if (uniqueVertices.count(vertex4p) == 0) {
                    uniqueVertices[vertex4p] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex4p);
                }

                // Triangle 1
                indices.push_back(uniqueVertices[vertex3p]);
                indices.push_back(uniqueVertices[vertex2p]);
                indices.push_back(uniqueVertices[vertex1p]);

                // Triangle 2
                indices.push_back(uniqueVertices[vertex4p]);
                indices.push_back(uniqueVertices[vertex2p]);
                indices.push_back(uniqueVertices[vertex3p]);
            }
        }

        builder = *new Model::Builder{vertices, indices};
    }
}