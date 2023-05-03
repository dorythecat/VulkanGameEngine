#ifndef TEXTURE_COMPONENT_HPP
#define TEXTURE_COMPONENT_HPP

#include "../component.hpp"

#include "../../texture/texture.hpp"

namespace Engine {
    class TextureComponent : public Component {
    public:
        std::shared_ptr<Texture> diffuseMap;

        TextureComponent(std::shared_ptr<Texture> diffuseMap) : diffuseMap(diffuseMap) {}
        ComponentType getComponentType() const override { return TEXTURE; }
    };
}

#endif
