#ifndef MOVEMENTCONTROLLER_HPP
#define MOVEMENTCONTROLLER_HPP

#include "GLFW/glfw3.h"

#include <limits>

#include "../../entity/entity.hpp"

namespace Engine {
    class MovementController {
    public:
        struct KeyMappings {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_SPACE;
            int moveDown = GLFW_KEY_LEFT_CONTROL;
        };

        KeyMappings keys{};
        float moveSpeed = 3.0f;
        float sensitivity = 0.1f;

        void move(GLFWwindow *window, float deltaTime, Entity &ent) const;
        void look(GLFWwindow *window, float deltaTime, Entity &ent) const;
    };
}

#endif