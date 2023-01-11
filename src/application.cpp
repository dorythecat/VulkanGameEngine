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

    std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset) {
        Model::Builder modelBuilder{};
        modelBuilder.vertices = {
                // left face (white)
                {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
                {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
                {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
                {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

                // right face (yellow)
                {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
                {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
                {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
                {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

                // top face (orange, remember y axis points down)
                {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
                {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
                {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
                {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

                // bottom face (red)
                {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
                {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
                {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
                {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

                // nose face (blue)
                {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
                {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
                {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
                {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

                // tail face (green)
                {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
                {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
                {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
                {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        }; for (auto& v : modelBuilder.vertices) v.position += offset;
        modelBuilder.indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                                12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};
        return std::make_unique<Model>(device, modelBuilder);
    }

    void Application::loadGameObjects () {
        std::shared_ptr<Model> cubeModel = createCubeModel(device, glm::vec3{0.0f, 0.0f, 0.0f});
        auto cube = GameObject::createGameObject();
        cube.model = cubeModel;
        cube.transform.position = glm::vec3{0.0f, 0.0f, 2.5f};
        cube.transform.scale = glm::vec3{0.5f, 0.5f, 0.5f};
        gameObjects.push_back(std::move(cube));
    }
}