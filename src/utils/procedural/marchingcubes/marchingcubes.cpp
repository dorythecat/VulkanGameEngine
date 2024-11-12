#include "marchingcubes.hpp"

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
    void MarchingCubes::generateModel() {
        // Reserve space for the vertices and indices. This is done to avoid reallocations, which should give us better performance.
        uint32_t reserveSpace = resolution * resolution * resolution;
        vertices.reserve(reserveSpace);
        indices.reserve(reserveSpace);
        std::unordered_map<Model::Vertex, uint32_t> uniqueVertices(reserveSpace);

        // Precomputed values for the loops.
        precision_t step = 2.0f / static_cast<precision_t>(resolution);

        // Loop through all the cells in the grid.
        for (uint32_t x = 0; x < resolution; x++) {
            for (uint32_t y = 0; y < resolution; y++) {
                for (uint32_t z = 0; z < resolution; z++) {
                    precision_t x0 = static_cast<precision_t>(x) * step - 1.0f;
                    precision_t y0 = static_cast<precision_t>(y) * step - 1.0f;
                    precision_t z0 = static_cast<precision_t>(z) * step - 1.0f;

                    precision_t x1 = x0 + step;
                    precision_t y1 = y0 + step;
                    precision_t z1 = z0 + step;

                    cell grid;

                    grid.val[0] = surface(x0, y0, z1);
                    grid.val[1] = surface(x1, y0, z1);
                    grid.val[2] = surface(x1, y0, z0);
                    grid.val[3] = surface(x0, y0, z0);
                    grid.val[4] = surface(x0, y1, z1);
                    grid.val[5] = surface(x1, y1, z1);
                    grid.val[6] = surface(x1, y1, z0);
                    grid.val[7] = surface(x0, y1, z0);

                    grid.p[0] = glm::vec3(x0, y0, z1);
                    grid.p[1] = glm::vec3(x1, y0, z1);
                    grid.p[2] = glm::vec3(x1, y0, z0);
                    grid.p[3] = glm::vec3(x0, y0, z0);
                    grid.p[4] = glm::vec3(x0, y1, z1);
                    grid.p[5] = glm::vec3(x1, y1, z1);
                    grid.p[6] = glm::vec3(x1, y1, z0);
                    grid.p[7] = glm::vec3(x0, y1, z0);

                    auto triangles = new triangle[5];

                    uint8_t n = polygonise(grid, triangles);
                    if (n == 0) continue;

                    // Translate from triangles to vertices.
                    for (uint8_t i = 0; i < n; i++) {
                        glm::vec3 normal = glm::normalize(glm::cross(triangles[i].p[1] - triangles[i].p[0], triangles[i].p[2] - triangles[i].p[0]));
                        
                        Model::Vertex vertex1{};
                        vertex1.position = triangles[i].p[0];
                        vertex1.color = { 1.0f, 1.0f, 1.0f };
                        vertex1.normal = normal;
                        vertex1.texCoord = { 0.0f, 0.0f };

                        Model::Vertex vertex2{};
                        vertex2.position = triangles[i].p[1];
                        vertex2.color = { 1.0f, 1.0f, 1.0f };
                        vertex2.normal = normal;
                        vertex2.texCoord = { 0.0f, 0.0f };

                        Model::Vertex vertex3{};
                        vertex3.position = triangles[i].p[2];
                        vertex3.color = { 1.0f, 1.0f, 1.0f };
                        vertex3.normal = normal;
                        vertex3.texCoord = { 0.0f, 0.0f };

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

                        indices.push_back(uniqueVertices[vertex1]);
                        indices.push_back(uniqueVertices[vertex2]);
                        indices.push_back(uniqueVertices[vertex3]);
                    } delete[] triangles;
                }
            }
        } builder = *new Model::Builder{vertices, indices};
    }

    uint8_t MarchingCubes::polygonise(cell grid, triangle *triangles) {
        uint8_t cubeIndex = 0;
        if (grid.val[0] - isolevel < std::numeric_limits<precision_t>::epsilon()) cubeIndex |= 1;
        if (grid.val[1] - isolevel < std::numeric_limits<precision_t>::epsilon()) cubeIndex |= 2;
        if (grid.val[2] - isolevel < std::numeric_limits<precision_t>::epsilon()) cubeIndex |= 4;
        if (grid.val[3] - isolevel < std::numeric_limits<precision_t>::epsilon()) cubeIndex |= 8;
        if (grid.val[4] - isolevel < std::numeric_limits<precision_t>::epsilon()) cubeIndex |= 16;
        if (grid.val[5] - isolevel < std::numeric_limits<precision_t>::epsilon()) cubeIndex |= 32;
        if (grid.val[6] - isolevel < std::numeric_limits<precision_t>::epsilon()) cubeIndex |= 64;
        if (grid.val[7] - isolevel < std::numeric_limits<precision_t>::epsilon()) cubeIndex |= 128;

        if (cubeIndex == 0 || cubeIndex == 255) return 0; // Cube is entirely in or out of the surface

        glm::vec3 vertices[12];
        uint16_t edge = edgeTable[cubeIndex];
        if (edge & 1) vertices[0] = interpolateVector(grid.p[0], grid.p[1], grid.val[0], grid.val[1]);
        if (edge & 2) vertices[1] = interpolateVector(grid.p[1], grid.p[2], grid.val[1], grid.val[2]);
        if (edge & 4) vertices[2] = interpolateVector(grid.p[2], grid.p[3], grid.val[2], grid.val[3]);
        if (edge & 8) vertices[3] = interpolateVector(grid.p[3], grid.p[0], grid.val[3], grid.val[0]);
        if (edge & 16) vertices[4] = interpolateVector(grid.p[4], grid.p[5], grid.val[4], grid.val[5]);
        if (edge & 32) vertices[5] = interpolateVector(grid.p[5], grid.p[6], grid.val[5], grid.val[6]);
        if (edge & 64) vertices[6] = interpolateVector(grid.p[6], grid.p[7], grid.val[6], grid.val[7]);
        if (edge & 128) vertices[7] = interpolateVector(grid.p[7], grid.p[4], grid.val[7], grid.val[4]);
        if (edge & 256) vertices[8] = interpolateVector(grid.p[0], grid.p[4], grid.val[0], grid.val[4]);
        if (edge & 512) vertices[9] = interpolateVector(grid.p[1], grid.p[5], grid.val[1], grid.val[5]);
        if (edge & 1024) vertices[10] = interpolateVector(grid.p[2], grid.p[6], grid.val[2], grid.val[6]);
        if (edge & 2048) vertices[11] = interpolateVector(grid.p[3], grid.p[7], grid.val[3], grid.val[7]);

        uint8_t n = 0;
        for (uint8_t i = 0; triTable[cubeIndex][i] >= 0; n++) {
            triangles[n].p[0] = vertices[triTable[cubeIndex][i++]];
            triangles[n].p[1] = vertices[triTable[cubeIndex][i++]];
            triangles[n].p[2] = vertices[triTable[cubeIndex][i++]];
        } return n;
    }

    precision_t MarchingCubes::clamp(precision_t x, precision_t edge0, precision_t edge1) {
        if (abs(x) < std::numeric_limits<precision_t>::epsilon()) return 0.0f;
        if (abs(x - 1.0f) < std::numeric_limits<precision_t>::epsilon()) return 1.0f;
        return x;
    }

    glm::vec3 MarchingCubes::interpolateVector(glm::vec3 p1, glm::vec3 p2, precision_t valp0, precision_t valp1) const {
        precision_t x0 = isolevel - valp0;
        precision_t x1 = valp1 - valp0;

        if (abs(x0) <= std::numeric_limits<precision_t>::epsilon() ||
            abs(x1) <= std::numeric_limits<precision_t>::epsilon()) return p1;
        if (abs(isolevel - valp1) <= std::numeric_limits<precision_t>::epsilon()) return p2;

        precision_t mu = clamp(x0 / x1, valp0, valp1);

        return {
            p1.x + mu * (p2.x - p1.x),
            p1.y + mu * (p2.y - p1.y),
            p1.z + mu * (p2.z - p1.z)
        };
    }

    precision_t MarchingCubes::testSurface(precision_t x, precision_t y, precision_t z) {
        // return x * x + y * y + z * z - 1.0f; // Sphere

        // Torus
        // precision_t a = x * x + y * y;
        // precision_t b = a + z * z + 0.25f - 0.04f;
        // return b * b - a;

        // Two metaballs
        precision_t a = x * x + y * y + z * z + 0.25f;
        return (2.0f / std::sqrt(a + x) + 1.0f / std::sqrt(a - x)) / 5.0f - 1.0f;
    }
}