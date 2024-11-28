#ifndef SIMPLERENDERSYSTEM_HPP
#define SIMPLERENDERSYSTEM_HPP

#include "../rendersystem.hpp"

namespace Engine {
    class SimpleRenderSystem final : RenderSystem {
    public:
        SimpleRenderSystem(Device &device,
                           VkRenderPass renderPass,
                           VkDescriptorSetLayout globalSetLayout) :
                RenderSystem(device,
                             renderPass,
                             globalSetLayout) { init(); }

        void render(FrameInfo &frameInfo) override;
        using RenderSystem::toggleWireframe;
    private:
        constexpr std::string vertPath() override { return "../res/shaders/compiled/standard.vert.spv"; }
        constexpr std::string fragPath() override { return "../res/shaders/compiled/standard.frag.spv"; }
    };
}

#endif