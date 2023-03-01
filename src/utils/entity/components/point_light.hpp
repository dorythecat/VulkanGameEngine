#ifndef POINT_LIGHT_COMPONENT_HPP
#define POINT_LIGHT_COMPONENT_HPP

#include "../component.hpp"

namespace Engine {
    class PointLightComponent : public Component {
    public:
        float intensity;

        PointLightComponent(float intensity) : intensity(intensity) {}
        ComponentType getComponentType() const override { return POINT_LIGHT; }
    };
}

#endif
