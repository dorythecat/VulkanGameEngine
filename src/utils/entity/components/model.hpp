#ifndef MODEL_COMPONENT_HPP
#define MODEL_COMPONENT_HPP

#include "../component.hpp"

namespace Engine {
    class ModelComponent final : public Component {
    public:
        std::shared_ptr<Model> model;

        explicit ModelComponent(const std::shared_ptr<Model> &model) : model(model) {}
        [[nodiscard]] ComponentType getComponentType() const override { return MODEL; }
    };
}

#endif
