#include "entity.hpp"

namespace Engine {
    void Entity::addComponent(std::unique_ptr<Component> component) {
        assert(component != nullptr && "The component cannot be null!");
        ComponentType type = component->getComponentType();
        assert(components.find(type) == components.end() &&
               "The entity already has a component of this type!");
        component->entity = this;
        components[type] = std::move(component);
        componentMask |= static_cast<ComponentType_t>(1 << type);
    }
    void Entity::removeComponent(ComponentType type) {
        assert(components.find(type) != components.end() &&
               "The entity does not have a component of this type!");
        components.erase(type);
        componentMask &= static_cast<ComponentType_t>(~(1 << type));
    }

    TransformComponent *Entity::getTransformComponent() {
        assert(hasComponent(ComponentType::TRANSFORM) && "Transform component does not exist for this entity!");
        return dynamic_cast<TransformComponent*>(components[ComponentType::TRANSFORM].get());
    }
    PointLightComponent *Entity::getPointLightComponent() {
        assert(hasComponent(ComponentType::POINT_LIGHT) && "Point light component does not exist for this entity!");
        return dynamic_cast<PointLightComponent*>(components[ComponentType::POINT_LIGHT].get());
    }
    ModelComponent *Entity::getModelComponent() {
        assert(hasComponent(ComponentType::MODEL) && "Model component does not exist for this entity!");
        return dynamic_cast<ModelComponent*>(components[ComponentType::MODEL].get());
    }
    TextureComponent *Entity::getTextureComponent() {
        assert(hasComponent(ComponentType::TEXTURE) && "Texture component does not exist for this entity!");
        return dynamic_cast<TextureComponent*>(components[ComponentType::TEXTURE].get());
    }

    Entity Entity::createEntity() {
        static id_t currentId = 0;
        return { currentId++ };
    }
    Entity Entity::createPointLightEntity(float intensity, float radius, glm::vec3 color) {
        Entity ent = Entity::createEntity();
        ent.addComponent(std::make_unique<TransformComponent>(glm::vec3(0.0f),
                                                              glm::vec3{radius, 1.0f, 1.0f}));
        ent.addComponent(std::make_unique<PointLightComponent>(intensity, color));
        return ent;
    }
}