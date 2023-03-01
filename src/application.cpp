#include "application.hpp"

namespace Engine {
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
                .addBinding(0,
                            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT).build();

        std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (unsigned int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            DescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        SimpleRenderSystem simpleRenderSystem{device,
                                              renderer.getSwapChainRenderPass(),
                                              globalSetLayout->getDescriptorSetLayout()};
        BillboardRenderSystem billboardRenderSystem{device,
                                                    renderer.getSwapChainRenderPass(),
                                                    globalSetLayout->getDescriptorSetLayout()};
        Camera camera{};
        camera.setViewTarget(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.5f, 0.0f, 1.0f});

        auto cameraEntity = Entity::createEntity();
        cameraEntity.addComponent(std::make_unique<TransformComponent>(glm::vec3{0.0f, 0.0f, -2.5f}));
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();
        while (!window.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            // Make the maximum time between frames 1/30 seconds, so the minimum framerate is 30 FPS
            deltaTime = glm::min(deltaTime, 0.0333f);

            cameraController.moveInPlaneXZ(window.getWindow(), deltaTime, cameraEntity);
            camera.setViewXYZ(cameraEntity.getTransformComponent()->position,
                              cameraEntity.getTransformComponent()->rotation);

            float aspectRatio = renderer.getAspectRatio();
            // camera.setOrthographicProjection(aspectRatio, -1.0f, -1.0f, 1.0f);
            // camera.setOrthographicProjection(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
            camera.setPerspectiveProjection(glm::radians(50.0f), aspectRatio, 0.1f, 100.0f);

            if (auto commandBuffer = renderer.beginFrame()) {
                int frameIndex = renderer.getCurrentFrameIndex();
                FrameInfo frameInfo{frameIndex,
                                    deltaTime,
                                    commandBuffer,
                                    camera,
                                    globalDescriptorSets[frameIndex],
                                    entities};

                // Update cycle
                GlobalUbo ubo{};
                ubo.projectionMatrix = frameInfo.camera.getProjectionMatrix();
                ubo.viewMatrix = frameInfo.camera.getViewMatrix();
                billboardRenderSystem.update(frameInfo, ubo);
                uboBuffers[frameInfo.frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameInfo.frameIndex]->flush();

                // Render cycle
                renderer.beginSwapChainRenderPass(frameInfo.commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo);
                billboardRenderSystem.render(frameInfo);
                renderer.endSwapChainRenderPass(frameInfo.commandBuffer);
                renderer.endFrame();
            }
        } vkDeviceWaitIdle(device.device()); // Wait for all the resource to be freed before destroying them
    }

    void Application::loadGameObjects () {
        // Flat shaded sphere (left)
        std::shared_ptr<Model> sphereFlatModel = Model::createModelFromFile(device, "../res/models/sphere/sphere_flat.obj");
        auto sphereFlat = Entity::createEntity();
        sphereFlat.addComponent(std::make_unique<ModelComponent>(sphereFlatModel));
        sphereFlat.addComponent(std::make_unique<TransformComponent>(glm::vec3{2.5f, 0.0f, 5.0f},
                                                                       glm::vec3{0.5f, 0.5f, 0.5f}));
        entities.emplace(sphereFlat.getId(), std::move(sphereFlat));

        // Smooth shaded sphere (right)
        std::shared_ptr<Model> sphereSmoothModel = Model::createModelFromFile(device, "../res/models/sphere/sphere_smooth.obj");
        auto sphereSmooth = Entity::createEntity();
        sphereSmooth.addComponent(std::make_unique<ModelComponent>(sphereSmoothModel));
        sphereSmooth.addComponent(std::make_unique<TransformComponent>(glm::vec3{-2.5f, 0.0f, 5.0f},
                                                                       glm::vec3{0.5f, 0.5f, 0.5f}));
        entities.emplace(sphereSmooth.getId(), std::move(sphereSmooth));

        // Procedural quad (center)
        Procedural::Quad q(device, 128);
        q.generateModel();
        std::shared_ptr<Model> terrainModel = q.getModel();
        auto quad = Entity::createEntity();
        quad.addComponent(std::make_unique<ModelComponent>(terrainModel));
        quad.addComponent(std::make_unique<TransformComponent>(glm::vec3{-2.5f, 0.0f, 5.0f},
                                                                glm::vec3{5.0f, 5.0f, 5.0f}));
        entities.emplace(quad.getId(), std::move(quad));

        // Procedural cube (center up)
        Procedural::Cube c(device, 128);
        c.generateModel();
        std::shared_ptr<Model> cubeModel = c.getModel();
        auto cube = Entity::createEntity();
        cube.addComponent(std::make_unique<ModelComponent>(cubeModel));
        cube.addComponent(std::make_unique<TransformComponent>(glm::vec3{-0.5f, -2.0f, 5.0f}));
        entities.emplace(cube.getId(), std::move(cube));

        // Point lights
        std::vector<glm::vec3> lightColors{
                {1.0f, 0.1f, 0.1f},
                {0.1f, 0.1f, 1.0f},
                {0.1f, 1.0f, 0.1f},
                {1.0f, 1.0f, 0.1f},
                {0.1f, 1.0f, 1.0f},
                {1.0f, 1.0f, 1.0f}
        };

        for (unsigned int i = 0; i < lightColors.size(); i++) {
            auto pointLight = Entity::createPointLight(1.0f);
            pointLight.color = lightColors[i];
            auto rotateLight = glm::rotate(
                    glm::mat4(1.0f),
                    (static_cast<float>(i) * glm::two_pi<float>()) / static_cast<float>(lightColors.size()),
                    {0.0f, -1.0f, 0.0f});
            pointLight.getTransformComponent()->position = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
            entities.emplace(pointLight.getId(), std::move(pointLight));
        }
    }
}