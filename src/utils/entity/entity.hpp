#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>

#include "../model/model.hpp"

// Components
#include "component.hpp"
#include "components/transform.hpp"
#include "components/point_light.hpp"
#include "components/model.hpp"
#include "components/texture.hpp"

namespace Engine {
    class Entity {
    public:
        typedef uint16_t id_t; // 65536 entities should do, at least for now
        typedef std::unordered_map<id_t, Entity> Map;

        Entity(const Entity &) = delete;
        Entity &operator=(const Entity &) = delete;
        Entity(Entity &&) = default;
        Entity &operator=(Entity &&) = default;

        [[nodiscard]] ComponentType_t getComponentMask() const { return componentMask; }
        [[nodiscard]] bool hasComponent(ComponentType type) const { return componentMask & 1 << type; }

        void addComponent(std::unique_ptr<Component> component);
        void removeComponent(ComponentType type);

        TransformComponent *getTransformComponent();
        PointLightComponent *getPointLightComponent();
        ModelComponent *getModelComponent();
        TextureComponent *getTextureComponent();

        static Entity createEntity();
        static Entity createPointLightEntity(float intensity = 1.0f,
                                             float radius = 1.0f,
                                             glm::vec3 color = {1.0f, 1.0f, 1.0f});

        [[nodiscard]] id_t getId() const { return id; }
    private:
        id_t id;
        ComponentType_t componentMask = 0;
        std::map<ComponentType, std::unique_ptr<Component>> components;

        // DO NOT MAKE THIS EXPLICIT
        // It will cause compile-time trouble
        Entity(const id_t id) : id(id) {}
    };
}

#endif