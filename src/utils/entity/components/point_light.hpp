#ifndef POINT_LIGHT_COMPONENT_HPP
#define POINT_LIGHT_COMPONENT_HPP

#include "../component.hpp"

namespace Engine {
    class PointLightComponent final : public Component {
    public:
        float intensity;
        glm::vec3 color;

        PointLightComponent(const float intensity, const glm::vec3 color) : intensity(intensity), color(color) {}
        [[nodiscard]] ComponentType getComponentType() const override { return POINT_LIGHT; }
    };
}

#endif
