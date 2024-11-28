#ifndef TEXTURE_COMPONENT_HPP
#define TEXTURE_COMPONENT_HPP

#include "../component.hpp"

#include "../../texture/texture.hpp"

namespace Engine {
    class TextureComponent final : public Component {
    public:
        std::shared_ptr<Texture> diffuseMap;

        explicit TextureComponent(const std::shared_ptr<Texture> &diffuseMap) : diffuseMap(diffuseMap) {}
        [[nodiscard]] ComponentType getComponentType() const override { return TEXTURE; }
    };
}

#endif
