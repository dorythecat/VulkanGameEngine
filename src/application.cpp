#include "application.hpp"

namespace Engine {
    Application::Application() {
        globalPool = DescriptorPool::Builder(device)
                .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
                .build();

        framePools.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
        const DescriptorPool::Builder framePoolBuilder =
            DescriptorPool::Builder(device)
                .setMaxSets(1024)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1024)
                .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1024)
                .setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
        for (auto &framePool : framePools) framePool = framePoolBuilder.build();
        loadEntities();
    }
    Application::~Application() {
        vkDeviceWaitIdle(device.device()); // Wait for all the resources to be freed before destroying them

        // We need our descriptor pools destroyed before the device is
        globalPool = nullptr;
        framePools.clear();

        _maindelqueue.flush();

        renderer.del();
        device.del();

        destroyImGUI();
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
        for (uint32_t i = 0; i < globalDescriptorSets.size(); i++) {
            VkDescriptorBufferInfo bufferInfo = uboBuffers[i]->descriptorInfo();
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
        TextureRenderSystem textureRenderSystem{device,
                                                   renderer.getSwapChainRenderPass(),
                                                   globalSetLayout->getDescriptorSetLayout()};

        Camera camera{};
        camera.setViewTarget(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.5f, 0.0f, 1.0f});

        auto cameraEntity = Entity::createEntity();
        cameraEntity.addComponent(std::make_unique<TransformComponent>(glm::vec3{0.0f, 0.0f, -2.5f}));
        MovementController movementController{};

        initImGUI();

        glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported()) glfwSetInputMode(window.getWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        bool centered = true;
        float aspectRatio = 0.0f;
        auto currentTime = std::chrono::high_resolution_clock::now();
        while (!window.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            deltaTime = glm::min(deltaTime, FrameInfo::MAX_DELTA_TIME);
            currentTime = newTime;

            if (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS && centered) {
                glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                centered = false;
            } else if (glfwGetMouseButton(window.getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !centered) {
                glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                centered = true;
            }

            if (aspectRatio != renderer.getAspectRatio()) {
                aspectRatio = renderer.getAspectRatio();

                // camera.setOrthographicProjection(aspectRatio, -1.0f, -1.0f, 1.0f);
                // camera.setOrthographicProjection(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
                camera.setPerspectiveProjection(FOV, aspectRatio, NEAR_PLANE, FAR_PLANE);
            }


            if (centered) {
                movementController.move(window.getWindow(), deltaTime, cameraEntity);
                movementController.look(window.getWindow(), deltaTime, cameraEntity);
                camera.setViewXYZ(cameraEntity.getTransformComponent()->position,
                                  cameraEntity.getTransformComponent()->rotation);
                if (glfwGetKey(window.getWindow(), GLFW_KEY_F5) == GLFW_PRESS) {
                    simpleRenderSystem.toggleWireframe();
                    textureRenderSystem.toggleWireframe();
                }
            }

            if (auto commandBuffer = renderer.beginFrame()) {
                uint32_t frameIndex = renderer.getCurrentFrameIndex();
                framePools[frameIndex]->resetPool();
                FrameInfo frameInfo{frameIndex,
                                    deltaTime,
                                    commandBuffer,
                                    camera,
                                    globalDescriptorSets[frameIndex],
                                    *framePools[frameIndex],
                                    entities};

                // Update cycle
                GlobalUbo ubo{};
                ubo.projectionMatrix = frameInfo.camera.getProjectionMatrix();
                ubo.viewMatrix = frameInfo.camera.getViewMatrix();
                ubo.inverseViewMatrix = frameInfo.camera.getInverseViewMatrix();

                ubo.ambientStrength = ambientStrength;
                ubo.diffuseStrength = diffuseStrength;
                ubo.specularStrength = specularStrength;
                ubo.shininess = shininess;

                ubo.texturesEnabled = texturesEnabled;

                billboardRenderSystem.update(frameInfo, ubo);
                uboBuffers[frameInfo.frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameInfo.frameIndex]->flush();

                // Render cycle
                renderer.beginSwapChainRenderPass(frameInfo.commandBuffer);

                // !!! ORDER MATTERS HERE !!!
                textureRenderSystem.render(frameInfo);
                simpleRenderSystem.render(frameInfo);
                billboardRenderSystem.render(frameInfo);

                drawImGUI(frameInfo);

                renderer.endSwapChainRenderPass(frameInfo.commandBuffer);
                renderer.endFrame();
            }
        } vkDeviceWaitIdle(device.device()); // Wait for all the resource to be freed before destroying them
    }

    void Application::initImGUI() {
        // TODO(Dory): Optimize pool sizes
        VkDescriptorPoolSize pool_sizes[] = {
                        { VK_DESCRIPTOR_TYPE_SAMPLER, 1024 },
                        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1024 },
                        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1024 },
                        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1024 },
                        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1024 },
                        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1024 },
                        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1024 },
                        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1024 },
                        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1024 },
                        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1024 },
                        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1024 }
        };

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1024;
        pool_info.poolSizeCount = std::size(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;

        VkDescriptorPool imguiPool;
        if (vkCreateDescriptorPool(device.device(), &pool_info, nullptr, &imguiPool) != VK_SUCCESS)
            throw std::runtime_error("Failed to create the ImGUI descriptor pool!");

        _maindelqueue.push_function([=, this]() {
            vkDestroyDescriptorPool(device.device(), imguiPool, nullptr);
        });


        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = device.instance();
        init_info.PhysicalDevice = device.physicalDevice();
        init_info.Device = device.device();
        init_info.Queue = device.graphicsQueue();
        init_info.DescriptorPool = imguiPool;
        init_info.MinImageCount = 2;
        init_info.ImageCount = SwapChain::MAX_FRAMES_IN_FLIGHT;
        init_info.MSAASamples = device.getDesiredSampleCount();
        init_info.CheckVkResultFn = [](VkResult err) {
            if (err != VK_SUCCESS) throw std::runtime_error("ImGUI Vulkan error!");
        };

        ImGui_ImplGlfw_InitForVulkan(window.getWindow(), true);
        ImGui_ImplVulkan_Init(&init_info, renderer.getSwapChainRenderPass());

        VkCommandBuffer commandBuffer = device.beginSingleTimeCommands();
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        device.endSingleTimeCommands(commandBuffer);

        vkDeviceWaitIdle(device.device());
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    void Application::drawImGUI(FrameInfo frameInfo) {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const auto windowOpen = new bool(true);
        constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;

        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Appearing);

        if (!ImGui::Begin("Debug Menu", windowOpen, windowFlags)) {
            ImGui::End();
            return;
        }

        if (ImGui::CollapsingHeader("Lightning")) {
            ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 1.0f);
            ImGui::SliderFloat("Diffuse Strength", &diffuseStrength, 0.0f, 1.0f);
            ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0f, 1.0f);
            ImGui::SliderFloat("Shininess", &shininess, 0.0f, 256.0f);
        }

        if (ImGui::CollapsingHeader("Textures")) {
            ImGui::Checkbox("Enable Texturing", &texturesEnabled);
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), frameInfo.commandBuffer);
    }

    void Application::destroyImGUI() {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Application::loadEntities() {
        entities.reserve(6);

        std::shared_ptr sphereFlatModel = Model::createModelFromFile(device, "../res/models/sphere/sphere_flat.obj");
        Entity sphereFlat = Entity::createEntity();
        sphereFlat.addComponent(std::make_unique<ModelComponent>(sphereFlatModel));
        sphereFlat.addComponent(std::make_unique<TransformComponent>(glm::vec3{2.5f, 0.0f, 5.0f},
                                                                       glm::vec3{0.5f, 0.5f, 0.5f}));
        entities.emplace(sphereFlat.getId(), std::move(sphereFlat));

        std::shared_ptr sphereSmoothModel = Model::createModelFromFile(device, "../res/models/sphere/sphere_smooth.obj");
        Entity sphereSmooth = Entity::createEntity();
        sphereSmooth.addComponent(std::make_unique<ModelComponent>(sphereSmoothModel));
        sphereSmooth.addComponent(std::make_unique<TransformComponent>(glm::vec3{-2.5f, 0.0f, 5.0f},
                                                                       glm::vec3{0.5f, 0.5f, 0.5f}));
        entities.emplace(sphereSmooth.getId(), std::move(sphereSmooth));

        Procedural::Terrain q(device, 2, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f});
        q.generateModel();
        std::shared_ptr quadModel = q.getModel();
        Entity quad = Entity::createEntity();
        quad.addComponent(std::make_unique<ModelComponent>(quadModel));
        quad.addComponent(std::make_unique<TextureComponent>(std::make_shared<Texture>(device, "../res/textures/texture.jpg")));
        quad.addComponent(std::make_unique<TransformComponent>(glm::vec3{-2.5f, 0.0f, 5.0f},
                                                                glm::vec3{5.0f, 5.0f, 5.0f}));
        entities.emplace(quad.getId(), std::move(quad));

        Procedural::Cube c(device, 128);
        c.generateModel();
        std::shared_ptr cubeModel = c.getModel();
        Entity cube = Entity::createEntity();
        cube.addComponent(std::make_unique<ModelComponent>(cubeModel));
        cube.addComponent(std::make_unique<TransformComponent>(glm::vec3{-0.5f, -2.0f, 5.0f}));
        entities.emplace(cube.getId(), std::move(cube));

        Procedural::MarchingCubes mc(device, 128, Procedural::MarchingCubes::testSurface, 0.0f);
        mc.generateModel();
        std::shared_ptr mcModel = mc.getModel();
        Entity mcEntity = Entity::createEntity();
        mcEntity.addComponent(std::make_unique<ModelComponent>(mcModel));
        mcEntity.addComponent(std::make_unique<TransformComponent>(glm::vec3{2.5f, -2.0f, 5.0f}));
        entities.emplace(mcEntity.getId(), std::move(mcEntity));

        Entity pointLight = Entity::createPointLightEntity();
        pointLight.getTransformComponent()->position = glm::vec3(0.0f, -3.0f, 3.0f);
        entities.emplace(pointLight.getId(), std::move(pointLight));
    }
}