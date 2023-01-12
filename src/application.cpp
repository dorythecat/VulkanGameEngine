#include "application.hpp"

namespace Engine {
    Application::Application() {
        loadGameObjects();
    }
    Application::~Application() = default;
    void Application::run() {
        SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};
        Camera camera{};
        camera.setViewTarget(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.5f, 0.0f, 1.0f});

        auto cameraObject = GameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();
        while (!window.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            glm::min(deltaTime, 0.0333f); // Make the maximum time between frames 1/30 seconds, so the minimum framerate is 30 FPS

            cameraController.moveInPlaneXZ(window.getWindow(), deltaTime, cameraObject);
            camera.setViewXYZ(cameraObject.transform.position, cameraObject.transform.rotation);

            float aspectRatio = renderer.getAspectRatio();
            // camera.setOrthographicProjection(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
            camera.setPerspectiveProjection(glm::radians(50.0f), aspectRatio, 0.1f, 10.0f);

            if (auto commandBuffer = renderer.beginFrame()) {
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }
        vkDeviceWaitIdle(device.device()); // Wait for all the resource to be freed before destroying them
    }

    void Application::loadGameObjects () {
        std::shared_ptr<Model> cubeModel = Model::createModelFromFile(device, "../res/models/cube/cube.obj");
        auto gameObject = GameObject::createGameObject();
        gameObject.model = cubeModel;
        gameObject.transform.position = glm::vec3{0.0f, 0.0f, 2.5f};
        gameObject.transform.scale = glm::vec3{0.5f, 0.5f, 0.5f};
        gameObjects.push_back(std::move(gameObject));
    }
}