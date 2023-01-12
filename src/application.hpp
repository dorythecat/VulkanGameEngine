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

#include "utils/window/window.hpp"
#include "utils/device/device.hpp"
#include "utils/gameobject/gameobject.hpp"
#include "utils/renderer/renderer.hpp"
#include "utils/input/keyboard_movement_controller/keyboardmovementcontroller.hpp"
#include "utils/descriptors/descriptors.hpp"

#include "rendersystems/simple/simplerendersystem.hpp"

namespace Engine {
    class Application {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        Application();
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void run();
    private:
        Window window{WIDTH, HEIGHT, "Vulkan test window"};
        Device device{window};
        Renderer renderer{window, device};
        std::vector<GameObject> gameObjects;

        std::unique_ptr<DescriptorPool> globalPool{};

        void loadGameObjects();
    };
}


#endif
