#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>

#include "../model/model.hpp"

#include "component.hpp"
#include "components/transform.hpp"
#include "components/point_light.hpp"
#include "components/model.hpp"

namespace Engine {
    class Entity {
    public:
        typedef uint16_t id_t; // 65536 entities should do, at least for now
        typedef std::unordered_map<id_t, Entity> Map;

        glm::vec3 color{};

        Entity(const Entity &) = delete;
        Entity &operator=(const Entity &) = delete;
        Entity(Entity &&) = default;
        Entity &operator=(Entity &&) = default;

        ComponentType_t getComponentMask() const { return componentMask; }
        bool hasComponent(ComponentType type) const { return componentMask & (1 << type); }

        void addComponent(std::unique_ptr<Component> component) {
            ComponentType type = component->getComponentType();
            assert(!hasComponent(type) && "Component already exists for this entity!");
            components[type] = std::move(component);
            componentMask |= static_cast<ComponentType_t>(1 << type);
        }
        void removeComponent(ComponentType type) {
            assert(hasComponent(type) && "Component does not exist for this entity!");
            components.erase(type);
            componentMask &= static_cast<ComponentType_t>(~(1 << type));
        }

        TransformComponent *getTransformComponent() {
            assert(hasComponent(ComponentType::TRANSFORM) && "Transform component does not exist for this entity!");
            return dynamic_cast<TransformComponent*>(components[ComponentType::TRANSFORM].get());
        }

        PointLightComponent *getPointLightComponent() {
            assert(hasComponent(ComponentType::POINT_LIGHT) && "Point light component does not exist for this entity!");
            return dynamic_cast<PointLightComponent*>(components[ComponentType::POINT_LIGHT].get());
        }

        ModelComponent *getModelComponent() {
            assert(hasComponent(ComponentType::MODEL) && "Model component does not exist for this entity!");
            return dynamic_cast<ModelComponent*>(components[ComponentType::MODEL].get());
        }

        static Entity createEntity();
        static Entity createPointLight(float intensity = 1.0f, float radius = 0.1f, glm::vec3 color = {1.0f, 1.0f, 1.0f});

        id_t getId() const { return id; }
    private:
        id_t id;
        ComponentType_t componentMask = 0;
        std::map<ComponentType, std::unique_ptr<Component>> components;

        Entity(id_t id) : id(id) {}
    };
}

#endif