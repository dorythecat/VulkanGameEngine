#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <cstdint>
#include <memory>
#include <map>
#include <cassert>

namespace Engine {
    typedef uint8_t ComponentType_t; // 8 different components should be enough for now

    enum ComponentType : ComponentType_t {
        TRANSFORM = 0,
        MODEL = 1,
        POINT_LIGHT = 2,
    };

    class Component {
    public:
        Component() = default;
        virtual ~Component() = default;

        Component(const Component &) = delete;
        Component &operator=(const Component &) = delete;
        Component(Component &&) = default;
        Component &operator=(Component &&) = default;

        virtual ComponentType getComponentType() const = 0;
    };
}

#endif
