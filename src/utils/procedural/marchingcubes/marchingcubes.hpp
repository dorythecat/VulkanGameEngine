#ifndef MARCHINGCUBES_HPP
#define MARCHINGCUBES_HPP

#include "tables.hpp"

#include "../procedural_mesh.hpp"

// See http://paulbourke.net/geometry/polygonise/ for original source code and algortihm description.
// See https://en.wikipedia.org/wiki/Implicit_surface for more information on implicit surfaces.

typedef float precision_t; // Technically redundant, but I would like to implement this on the entirety of the engine, this seems like a good starting point...

// A triangle
typedef struct {
    glm::vec3 p[3];
} triangle;

// A cubic cell
typedef struct {
    glm::vec3 p[8];
    precision_t val[8];
} cell;

namespace Engine::Procedural {
    class MarchingCubes : public ProceduralMesh {
    public:
        // For this algorithm, we take the standard inputs, plus a surface function whose inputs are in the range 0..1,
        // centered at (0, 0, 0), and an "isolevel”, or threshold.

        // Technically speaking, we could make the surface function take in a vec3,
        // but I don't know if it would really be that much cleaner or flexible, so...

        MarchingCubes(Device &device, uint32_t resolution, precision_t (*surface)(precision_t, precision_t, precision_t), precision_t isolevel) :
        ProceduralMesh(device, resolution), surface(surface), isolevel(isolevel) {
            // Some specific sanity checks
            assert(surface != nullptr && "Surface function must be defined!"); // Otherwise, we won't have a surface to render!
            assert(resolution > 1 && "Resolution must be greater than 1!"); // Otherwise, we won't have any triangles!
        }

        void generateModel() override;

        static precision_t testSurface(precision_t x, precision_t y, precision_t z);
    private:
        precision_t (*surface)(precision_t, precision_t, precision_t);
        precision_t isolevel;

        static uint8_t polygonise(precision_t isolevel, cell grid, triangle *triangles);

        static precision_t interpolate(precision_t x, precision_t edge0 = 0.0f, precision_t edge1 = 1.0f);
        static glm::vec3 interpolateVector(precision_t isolevel, glm::vec3 p1, glm::vec3 p2, precision_t valp1, precision_t valp2);
    };
}

#endif