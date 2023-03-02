#include "entity.hpp"

namespace Engine {
    Entity Entity::createEntity() {
        static id_t currentId = 0;
        return {currentId++};
    }
    Entity Entity::createPointLightEntity(float intensity, float radius, glm::vec3 color) {
        Entity ent = Entity::createEntity();
        ent.color = color;
        ent.addComponent(std::make_unique<TransformComponent>(glm::vec3(0.0f),
                                                              glm::vec3{radius, 1.0f, 1.0f}));
        ent.addComponent(std::make_unique<PointLightComponent>(intensity));
        return ent;
    }
}