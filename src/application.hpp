#ifndef APPLICATION_H
#define APPLICATION_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <memory>
#include <chrono>
#include <vector>
#include <array>

// ImGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

// Misc utils
#include "utils/window/window.hpp"
#include "utils/device/device.hpp"
#include "utils/entity/entity.hpp"
#include "utils/renderer/renderer.hpp"
#include "utils/input/keyboardmovementcontroller/keyboardmovementcontroller.hpp"
#include "utils/descriptors/descriptors.hpp"
#include "utils/texture/texture.hpp"
#include "utils/entity/components/texture.hpp"

// Procedural geometry
#include "utils/procedural/quad/quad.hpp"
#include "utils/procedural/cube/cube.hpp"
#include "utils/procedural/marchingcubes/marchingcubes.hpp"

// Render systems
#include "rendersystems/simple/simplerendersystem.hpp"
#include "rendersystems/billboard/billboardrendersystem.hpp"
#include "rendersystems/texture/texturerendersystem.hpp"

namespace Engine {
    class Application {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        static constexpr float FOV = glm::radians(60.0f);

        static constexpr float NEAR_PLANE = 0.1f;
        static constexpr float FAR_PLANE = 100.0f;

        // ImGUI control variables
        float ambientStrength = 1.0f;
        float diffuseStrength = 1.0f;
        float specularStrength = 1.0f;
        float shininess = 32.0f;

        bool texturesEnabled = true;

        Application();
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void run();
    private:
        Window window{WIDTH, HEIGHT, "Vulkan test window"};
        Device device{window};
        Renderer renderer{window, device};
        Entity::Map entities;

        std::unique_ptr<DescriptorPool> globalPool{};
        std::vector<std::unique_ptr<DescriptorPool>> framePools;

        // ImGUI
        void initImGUI();
        void drawImGUI(FrameInfo frameInfo);
        void destroyImGUI();

        void loadEntities();
    };
}


#endif
