#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include <glm/gtc/matrix_transform.hpp>

#include "../component.hpp"

namespace Engine {
    class TransformComponent : public Component {
    public:
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::vec3 rotation{0.0f, 0.0f, 0.0f};

        TransformComponent() = default;
        TransformComponent(const glm::vec3 position) :
                           position(position) {}
        TransformComponent(const glm::vec3 position, const glm::vec3 scale) :
                           position(position), scale(scale) {}
        TransformComponent(const glm::vec3 position, const glm::vec3 scale, const glm::vec3 rotation) :
                           position(position), scale(scale), rotation(rotation) {}
        ComponentType getComponentType() const override { return TRANSFORM; }

        // Matrix corresponds to Translate * Rx * Ry * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 mat4() const {
            glm::mat4 mat{1.0f};
            mat = glm::translate(mat, position);
            mat = glm::rotate(mat, rotation.x, {1.0f, 0.0f, 0.0f});
            mat = glm::rotate(mat, rotation.y, {0.0f, 1.0f, 0.0f});
            mat = glm::rotate(mat, rotation.z, {0.0f, 0.0f, 1.0f});
            mat = glm::scale(mat, scale);
            return mat;
        }

        // The normal matrix is the inverse transpose of the model matrix
        // This can also be defined as R * S^-1
        glm::mat3 normal() const {
                const float s1 = glm::sin(rotation.x);
                const float c1 = glm::cos(rotation.x);

                const float s2 = glm::sin(rotation.y);
                const float c2 = glm::cos(rotation.y);

                const float s3 = glm::sin(rotation.z);
                const float c3 = glm::cos(rotation.z);

                const float c3s2 = c3 * s2;
                const float s3s2 = s3 * s2;

                const float xinv = 1.0f / scale.x;
                const float yinv = 1.0f / scale.y;
                const float zinv = 1.0f / scale.z;

                const float xs1 = xinv * s1;
                const float xc1 = xinv * c1;

                const float ys1 = yinv * s1;
                const float yc1 = yinv * c1;

                const float zc2 = zinv * c2;

                return glm::mat3{{
                        xinv * c2 * c3,
                                xc1 * s3 + xs1 * c3s2,
                                xs1 * s3 - xc1 * c3s2,
                    }, {
                        yinv * -c2 * s3,
                                yc1 * c3 - ys1 * s3s2,
                                ys1 * c3 + yc1 * s3s2,
                    }, {
                        zinv * s2,
                        zc2 * -s1,
                                zc2 * c1,
                    }};
        }
    };
}

#endif