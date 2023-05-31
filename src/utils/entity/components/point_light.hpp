#ifndef POINT_LIGHT_COMPONENT_HPP
#define POINT_LIGHT_COMPONENT_HPP

#include "../component.hpp"

namespace Engine {
    class PointLightComponent : public Component {
    public:
        float intensity;
        glm::vec3 color;

        PointLightComponent(float intensity, glm::vec3 color) : intensity(intensity), color(color) {}
        ComponentType getComponentType() const override { return POINT_LIGHT; }
    };
}

#endif
