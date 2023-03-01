#ifndef MODEL_COMPONENT_HPP
#define MODEL_COMPONENT_HPP

#include "../component.hpp"

namespace Engine {
    class ModelComponent : public Component {
    public:
        std::shared_ptr<Model> model;

        ModelComponent(std::shared_ptr<Model> model) : model(model) {}

        ComponentType getComponentType() const override { return MODEL; }
    };
}

#endif
