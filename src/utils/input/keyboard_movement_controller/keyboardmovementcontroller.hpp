#ifndef KEYBOARDMOVEMENTCONTROLLER_HPP
#define KEYBOARDMOVEMENTCONTROLLER_HPP

#include "GLFW/glfw3.h"

#include <limits>

#include "../../entity/entity.hpp"

namespace Engine {
    class KeyboardMovementController {
    public:
        struct KeyMappings {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_SPACE;
            int moveDown = GLFW_KEY_LEFT_CONTROL;

            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
        };

        KeyMappings keys{};
        float moveSpeed = 3.0f;
        float lookSpeed = 1.5f;

        void moveInPlaneXZ(GLFWwindow *window, float deltaTime, Entity &gameObject);
    };
}

#endif