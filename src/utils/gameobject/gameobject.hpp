#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <memory>

#include "../model/model.hpp"

// TODO: Update to an ECS system
// GameObject systems are outdated and are too object-oriented for my taste
// That's what you get with a sassy engineer :P

// TODO: Update to the third dimension
// 2D is plain, long live 3D
namespace Engine {
    struct Transform2DComponent {
        glm::vec2 position{};
        glm::vec2 scale{};
        float rotation = 0.0f;

        glm::mat2 mat2() {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMat{{c, s}, {-s, c}};
            glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};
            return rotMat * scaleMat;
        }
    };

    class GameObject {
    public:
        using id_t = unsigned int;

        std::shared_ptr<Model> model{};
        glm::vec3 color{};

        Transform2DComponent transform2D{};

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

        explicit GameObject(id_t objId) : id(objId) {}
    };
}

#endif