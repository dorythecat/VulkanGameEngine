#include <cassert>
#include "window.hpp"

namespace Engine {
    Window::Window(int width, int height, const char* title) :
            width(width), height(height), window_title(title) {
        init();
    }

    Window::~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Window::init() {
        assert(glfwInit() && "Failed to initialize GLFW!");
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        glfwWindowHint(GLFW_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_VERSION_REVISION, 8);

        // glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // https://github.com/Dorycraft/VulkanGameEngine/issues/4

        window = glfwCreateWindow(width, height, window_title, nullptr, nullptr);

        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    bool Window::shouldClose () const { return glfwWindowShouldClose(window); }

    void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
            throw std::runtime_error("Failed to create the window surface!");
    }

    void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height){
        auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        win->framebufferResized = true;
        win->width = width;
        win->height = height;
    }
}