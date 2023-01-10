#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

#include "../model/model.hpp"

// TODO: Update to an ECS system
// GameObject systems are outdated and are too object-oriented for my taste
// That's what you get with a sassy engineer :P

// TODO: Update to the third dimension
// 2D is plain, long live 3D
namespace Engine {
    struct TransformComponent {
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::vec3 rotation{0.0f, 0.0f, 0.0f};

        // Matrix corrsponds to Translate * Rx * Ry * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 mat4() const {
            const float s1 = glm::sin(rotation.x);
            const float c1 = glm::cos(rotation.x);

            const float s2 = glm::sin(rotation.y);
            const float c2 = glm::cos(rotation.y);

            const float s3 = glm::sin(rotation.z);
            const float c3 = glm::cos(rotation.z);

            const float c3s2 = c3 * s2;
            const float s3s2 = s3 * s2;

            const float xs1 = scale.x * s1;
            const float xc1 = scale.x * c1;

            const float ys1 = scale.y * s1;
            const float yc1 = scale.y * c1;

            const float zc2 = scale.z * c2;

            return glm::mat4{{
                            scale.x * c2 * c3,
                            xc1 * s3 + xs1 * c3s2,
                            xs1 * s3 - xc1 * c3s2,
                            0.0f,
                    }, {
                            scale.y * -c2 * s3,
                            yc1 * c3 - ys1 * s3s2,
                            ys1 * c3 + yc1 * s3s2,
                            0.0f,
                    }, {
                            scale.z * s2,
                            zc2 * -s1,
                            zc2 * c1,
                            0.0f,
                    }, {position.x, position.y, position.z, 1.0f}};
        }
    };

    class GameObject {
    public:
        using id_t = unsigned int;

        std::shared_ptr<Model> model{};
        glm::vec3 color{};

        TransformComponent transform{};

        static GameObject createGameObject() {
            static id_t currentId = 0;
            return {currentId++};
        }

        GameObject(const GameObject &) = delete;
        GameObject &operator=(const GameObject &) = delete;
        GameObject(GameObject &&) = default;
        GameObject &operator=(GameObject &&) = default;

        id_t getId() const { return id; }
    private:
        id_t id;

        GameObject(id_t objId) : id(objId) {}
    };
}

#endif