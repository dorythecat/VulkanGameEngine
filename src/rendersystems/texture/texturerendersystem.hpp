#ifndef TEXTURERENDERSYSTEM_HPP
#define TEXTURERENDERSYSTEM_HPP

#include "../rendersystem.hpp"

namespace Engine {
    class TextureRenderSystem final : RenderSystem {
    public:
        TextureRenderSystem(Device &device,
                            VkRenderPass renderPass,
                            VkDescriptorSetLayout globalSetLayout) :
                RenderSystem(device,
                             renderPass,
                             globalSetLayout) { init(); }

        void render(FrameInfo &frameInfo) override;
        using RenderSystem::toggleWireframe;
    private:
        constexpr std::string vertPath() override { return "../res/shaders/compiled/texture.vert.spv"; }
        constexpr std::string fragPath() override { return "../res/shaders/compiled/texture.frag.spv"; }

        std::unique_ptr<DescriptorSetLayout> renderSystemLayout;

        void createPipelineLayout() override;
    };
}

#endif