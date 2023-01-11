#include "keyboardmovementcontroller.hpp"

namespace Engine {
    void KeyboardMovementController::moveInPlaneXZ(GLFWwindow *window, float deltaTime, GameObject &gameObject) {
        glm::vec3 rotate{0.0f, 0.0f, 0.0f};

        if(glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.0f;
        else if(glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.0f;

        if(glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.0f;
        else if(glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.0f;

        // We check against the epsilon constant to avoid comparing directly to 0.0f
        // This is to avoid floating point errors
        if(glm::abs(rotate.x) > std::numeric_limits<float>::epsilon() ||
           glm::abs(rotate.y) > std::numeric_limits<float>::epsilon() ||
           glm::abs(rotate.z) > std::numeric_limits<float>::epsilon()) {
            // We normalize the rotation angles so that rotation in both axis are equivalent
            gameObject.transform.rotation += glm::normalize(rotate) * lookSpeed * deltaTime;

            // We clamp the x value and wrap the y value to keep the camera stable and avoid floating point errors
            gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
            gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());
        }

        float yaw = gameObject.transform.rotation.y;
        const glm::vec3 forwardVector{glm::sin(yaw), 0.0f, glm::cos(yaw)};
        const glm::vec3 rightVector{forwardVector.z, 0.0f, -forwardVector.x};
        const glm::vec3 upVector{0.0f, -1.0f, 0.0f};

        glm::vec3 moveVector{0.0f, 0.0f, 0.0f};

        if(glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveVector += forwardVector;
        else if(glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveVector -= forwardVector;

        if(glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveVector += rightVector;
        else if(glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveVector -= rightVector;

        if(glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveVector += upVector;
        else if(glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveVector -= upVector;

        if(glm::abs(moveVector.x) > std::numeric_limits<float>::epsilon() ||
           glm::abs(moveVector.y) > std::numeric_limits<float>::epsilon() ||
           glm::abs(moveVector.z) > std::numeric_limits<float>::epsilon()) {
            gameObject.transform.position += glm::normalize(moveVector) * moveSpeed * deltaTime;
        }
    }
}