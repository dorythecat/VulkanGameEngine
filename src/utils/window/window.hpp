#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <stdexcept>
#include <iostream>

namespace Engine {
    class Window {
    public:
        Window(int width, int height, const char* title);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool shouldClose() const;
        VkExtent2D getExtent() const { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
        bool wasWindowResized() const { return framebufferResized; }
        void resetWindowResizedFlag() { framebufferResized = false; }
        GLFWwindow* getWindow() const { return window; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
    private:
        int width, height;
        bool framebufferResized = false;

        const char *window_title = "Vulkan test window";

        GLFWwindow *window;

        void init();
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    };
}

#endif