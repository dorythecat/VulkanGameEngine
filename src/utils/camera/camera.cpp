#include "camera.hpp"

#include <cassert>
#include <limits>

namespace Engine {
    void Camera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far) {
        const float rl = 1.0f / (right - left);
        const float bt = 1.0f / (bottom - top);
        const float fn = 1.0f / (far - near);

        projectionMatrix = glm::mat4{1.0f};

        projectionMatrix[0][0] = 2.0f * rl;
        projectionMatrix[1][1] = 2.0f * bt;
        projectionMatrix[2][2] = fn;

        projectionMatrix[3][0] = (right + left) * -rl;
        projectionMatrix[3][1] = (bottom + top) * -bt;
        projectionMatrix[3][2] = near * -fn;
    }

    void Camera::setPerspectiveProjection(float fov, float aspect, float near, float far) {
        assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
        const float tanHalfFov = glm::tan(fov / 2.0f);

        const float ffn = far / (far - near);

        projectionMatrix = glm::mat4{0.0f};

        projectionMatrix[0][0] = 1.0f / (aspect * tanHalfFov);
        projectionMatrix[1][1] = 1.0f / tanHalfFov;
        projectionMatrix[2][2] = ffn;
        projectionMatrix[2][3] = 1.0f;
        projectionMatrix[3][2] = near * -ffn;
    }
}