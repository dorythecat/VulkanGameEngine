#include "movementcontroller.hpp"

namespace Engine {
    void MovementController::move(GLFWwindow *window, float deltaTime, Entity &ent) const {
        float yaw = ent.getTransformComponent()->rotation.y;
        const glm::vec3 forwardVector{glm::sin(yaw), 0.0f, glm::cos(yaw)};
        const glm::vec3 rightVector{forwardVector.z, 0.0f, -forwardVector.x};
        const glm::vec3 upVector{0.0f, -1.0f, 0.0f};

        glm::vec3 moveVector {0.0f, 0.0f, 0.0f};

        if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveVector += forwardVector;
        else if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveVector -= forwardVector;

        if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveVector += rightVector;
        else if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveVector -= rightVector;

        if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveVector += upVector;
        else if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveVector -= upVector;

        // We check against the epsilon constant to avoid comparing directly to 0.0f
        // This is to avoid floating point errors
        if (glm::abs(moveVector.x) >= std::numeric_limits<float>::epsilon() ||
           glm::abs(moveVector.y) >= std::numeric_limits<float>::epsilon() ||
           glm::abs(moveVector.z) >= std::numeric_limits<float>::epsilon()) {
            ent.getTransformComponent()->position += glm::normalize(moveVector) * moveSpeed * deltaTime;
        }
    }

    void MovementController::look(GLFWwindow *window, float deltaTime, Entity &ent) const {
        glm::vec2 rotation {0.0f, 0.0f};

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        glfwSetCursorPos(window, 0.0, 0.0);

        rotation.x = -static_cast<float>(ypos) * sensitivity * deltaTime;
        rotation.y = static_cast<float>(xpos) * sensitivity * deltaTime;

        // We check against the epsilon constant to avoid comparing directly to 0.0f
        // This is to avoid floating point errors
        if (glm::abs(rotation.x) >= std::numeric_limits<float>::epsilon() ||
           glm::abs(rotation.y) >= std::numeric_limits<float>::epsilon()) {

            // We clamp the x value and wrap the y value to keep the camera stable and avoid floating point errors
            ent.getTransformComponent()->rotation.x = glm::clamp(ent.getTransformComponent()->rotation.x + rotation.x, -1.5f, 1.5f);
            ent.getTransformComponent()->rotation.y = glm::mod(ent.getTransformComponent()->rotation.y + rotation.y, glm::two_pi<float>());
        }
    }
}