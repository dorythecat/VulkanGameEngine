#include "application.hpp"

namespace Engine {
    struct GlobalUbo {
        glm::mat4 projectionMatrix{1.0f};
        glm::mat4 viewMatrix{1.0f};
        glm::vec3 lightDirection = glm::normalize(glm::vec3{1.0f, -3.0f, -1.0f});
    };

    Application::Application() {
        globalPool = DescriptorPool::Builder(device)
                .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
                .build();
        loadGameObjects();
    }
    Application::~Application() {
        globalPool = nullptr; // We need globalPool to be destroyed before device is, this ensures that happens
    }

    void Application::run() {
        std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (auto &uboBuffer : uboBuffers) {
            uboBuffer = std::make_unique<Buffer>(
                    device,
                    sizeof(GlobalUbo),
                    1,
                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            uboBuffer->map();
        }

        auto globalSetLayout = DescriptorSetLayout::Builder(device)
                .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT)
                .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            DescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        SimpleRenderSystem simpleRenderSystem{device,
                                              renderer.getSwapChainRenderPass(),
                                              globalSetLayout->getDescriptorSetLayout()};
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
                int frameIndex = renderer.getCurrentFrameIndex();
                FrameInfo frameInfo{frameIndex,
                                    deltaTime,
                                    commandBuffer,
                                    camera,
                                    globalDescriptorSets[frameIndex]};

                // Update cycle
                GlobalUbo ubo{};
                ubo.projectionMatrix = frameInfo.camera.getProjectionMatrix();
                ubo.viewMatrix = frameInfo.camera.getViewMatrix();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);

                // Render cycle
                renderer.beginSwapChainRenderPass(frameInfo.commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
                renderer.endSwapChainRenderPass(frameInfo.commandBuffer);
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