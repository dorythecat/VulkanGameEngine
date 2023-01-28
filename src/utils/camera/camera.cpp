#include "camera.hpp"

#include <cassert>
#include <limits>

namespace Engine {
    void Camera::setOrthographicProjection(float right, float top, float near, float far) {
        // TODO(Dory): We should probably check for more assertion cases here, mainly to avoid 1.0f / 0.0f and similar
        assert(far > near && "Far plane cannot be closer than the near plane!");
        assert(far - near > std::numeric_limits<float>::epsilon() && "Near and far planes cannot be equal!");

        const float r = 1.0f / right;
        const float b = 1.0f / top;
        const float fn = -1.0f / (far - near);

        projectionMatrix = glm::mat4{0.0f};

        projectionMatrix[0][0] = r;
        projectionMatrix[1][1] = b;
        projectionMatrix[2][2] = 2.0f * fn;

        projectionMatrix[3][2] = (far + near) * fn;
        projectionMatrix[3][3] = 1.0f;
    }

    void Camera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far) {
        // TODO(Dory): We should probably check for more assertion cases here, mainly to avoid 1.0f / 0.0f and similar
        assert(far > near && "Far plane cannot be closer than the near plane!");
        assert(far - near > std::numeric_limits<float>::epsilon() && "Near and far planes cannot be equal!");

        const float rl = 1.0f / (right - left);
        const float bt = 1.0f / (bottom - top);
        const float fn = 1.0f / (far - near);

        projectionMatrix = glm::mat4{0.0f};

        projectionMatrix[0][0] = 2.0f * rl;
        projectionMatrix[1][1] = 2.0f * bt;
        projectionMatrix[2][2] = fn;

        projectionMatrix[3][0] = (right + left) * -rl;
        projectionMatrix[3][1] = (bottom + top) * -bt;
        projectionMatrix[3][2] = near * -fn;
        projectionMatrix[3][3] = 1.0f;
    }

    void Camera::setPerspectiveProjection(float fov, float aspect, float near, float far) {
        assert(fov > std::numeric_limits<float>::epsilon() && "Field of view cannot be zero or negative!");
        assert(aspect > std::numeric_limits<float>::epsilon() && "Aspect ratio cannot be zero or negative!");
        assert(near > std::numeric_limits<float>::epsilon() && "Near plane cannot be zero or negative!");
        assert(far > std::numeric_limits<float>::epsilon() && "Far plane cannot be zero or negative!");

        assert(far > near && "Far plane cannot be closer than the near plane!");
        assert(far - near > std::numeric_limits<float>::epsilon() && "Near and far planes cannot be equal!");

        projectionMatrix = glm::perspective(fov, aspect, near, far);

        // IDK why, but these two values are inverted in the perspective projection matrix from GLM
        // TODO(Dory): Check if this is a bug in GLM or if I'm just doing something wrong
        projectionMatrix[2][2] *= -1.0f;
        projectionMatrix[2][3] *= -1.0f;
    }

    void Camera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
        viewMatrix = glm::lookAt(position, target, up);
    }

    void Camera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
        setViewTarget(position, position + direction, up);
    }

    void Camera::setViewXYZ(glm::vec3 position, glm::vec3 rotation) {
        viewMatrix = glm::mat4{1.0f};
        viewMatrix = glm::rotate(viewMatrix, -rotation.x, glm::vec3{1.0f, 0.0f, 0.0f});
        viewMatrix = glm::rotate(viewMatrix, -rotation.y, glm::vec3{0.0f, 1.0f, 0.0f});
        viewMatrix = glm::rotate(viewMatrix, -rotation.z, glm::vec3{0.0f, 0.0f, 1.0f});
        viewMatrix = glm::translate(viewMatrix, -position);
    }
}