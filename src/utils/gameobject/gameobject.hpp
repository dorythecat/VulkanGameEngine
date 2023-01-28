#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>

#include "../model/model.hpp"

// TODO(Dory): Update to an ECS system
// GameObject systems are outdated and are too object-oriented for my taste
// That's what you get with a sassy engineer :P
namespace Engine {
    struct TransformComponent {
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::vec3 rotation{0.0f, 0.0f, 0.0f};

        glm::mat4 mat4() const;
        glm::mat3 normal() const;
    };

    struct PointLightComponent {
        float intensity = 1.0f;
    };

    class GameObject {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, GameObject>;

        glm::vec3 color{};

        // Required components
        TransformComponent transform{};

        // Optional components
        std::shared_ptr<Model> model{};
        std::unique_ptr<PointLightComponent> pointLight = nullptr;

        GameObject(const GameObject &) = delete;
        GameObject &operator=(const GameObject &) = delete;
        GameObject(GameObject &&) = default;
        GameObject &operator=(GameObject &&) = default;

        static GameObject createGameObject();
        static GameObject createPointLight(float intensity = 1.0f, float radius = 0.1f, glm::vec3 color = {1.0f, 1.0f, 1.0f});

        id_t getId() const { return id; }
    private:
        id_t id;

        GameObject(id_t objId) : id(objId) {}
    };
}

#endif