#include "application.hpp"

namespace Engine {
    Application::Application() {
        loadGameObjects();
    }
    Application::~Application() {}
    void Application::run() {
        SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};
        while (!window.shouldClose()) {
            glfwPollEvents();
            if (auto commandBuffer = renderer.beginFrame()) {
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }
        vkDeviceWaitIdle(device.device()); // Wait for all the resources to be freed before destroying them
    }
    void Application::loadGameObjects () {
        std::vector<Model::Vertex> vertices = {
                {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
        auto model = std::make_shared<Model>(device, vertices);
        auto triangle = GameObject::createGameObject();
        triangle.model = model;
        triangle.color = {1.0f, 0.2f, 0.1f};
        triangle.transform2D.position.x = 0.2f;
        triangle.transform2D.scale = {1.0f, 1.0f};
        triangle.transform2D.rotation = glm::pi<float>() / 2.0f;
        gameObjects.push_back(std::move(triangle));
    }
}