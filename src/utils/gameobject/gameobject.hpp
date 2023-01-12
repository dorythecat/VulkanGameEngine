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

        glm::mat4 mat4() const;
        glm::mat3 normal() const;
    };

    class GameObject {
    public:
        using id_t = unsigned int;

        std::shared_ptr<Model> model{};
        glm::vec3 color{};

        TransformComponent transform{};

        static GameObject createGameObject();

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