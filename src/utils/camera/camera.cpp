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

    void Camera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
        const glm::vec3 w{glm::normalize(direction)};
        const glm::vec3 u{glm::normalize(glm::cross(w, up))};
        const glm::vec3 v{glm::cross(w, u)};

        viewMatrix = glm::mat4{1.0f};
        viewMatrix[0][0] = u.x;
        viewMatrix[1][0] = u.y;
        viewMatrix[2][0] = u.z;
        viewMatrix[0][1] = v.x;
        viewMatrix[1][1] = v.y;
        viewMatrix[2][1] = v.z;
        viewMatrix[0][2] = w.x;
        viewMatrix[1][2] = w.y;
        viewMatrix[2][2] = w.z;
        viewMatrix[3][0] = -glm::dot(u, position);
        viewMatrix[3][1] = -glm::dot(v, position);
        viewMatrix[3][2] = -glm::dot(w, position);
    }

    void Camera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
        setViewDirection(position, target - position, up);
    }

    void Camera::setViewXYZ(glm::vec3 position, glm::vec3 rotation) {
        const float s1 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.x);

        const float s2 = glm::sin(rotation.y);
        const float c2 = glm::cos(rotation.y);

        const float s3 = glm::sin(rotation.z);
        const float c3 = glm::cos(rotation.z);

        const float c3s2 = c3 * s2;
        const float s3s2 = s3 * s2;

        const glm::vec3 u{c2 * c3, c1 * s3 + s1 * c3s2, s1 * s3 - c1 * c3s2};
        const glm::vec3 v{-c2 * s3, c1 * c3 - s1 * s3s2, s1 * c3 + c1 * s3s2};
        const glm::vec3 w{s2, -c2 * s1, c1 * c2};

        viewMatrix = glm::mat4{1.0f};
        viewMatrix[0][0] = u.x;
        viewMatrix[1][0] = u.y;
        viewMatrix[2][0] = u.z;
        viewMatrix[0][1] = v.x;
        viewMatrix[1][1] = v.y;
        viewMatrix[2][1] = v.z;
        viewMatrix[0][2] = w.x;
        viewMatrix[1][2] = w.y;
        viewMatrix[2][2] = w.z;
        viewMatrix[3][0] = -glm::dot(u, position);
        viewMatrix[3][1] = -glm::dot(v, position);
        viewMatrix[3][2] = -glm::dot(w, position);
    }
}