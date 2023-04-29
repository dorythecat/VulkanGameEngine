#include "camera.hpp"

#include <cassert>
#include <limits>

namespace Engine {
    void Camera::setOrthographicProjection(float right, float top, float near, float far) {
        assert(right > std::numeric_limits<float>::epsilon() && "Right plane must be positive!");
        assert(top > std::numeric_limits<float>::epsilon() && "Top plane must be positive!");
        assert(far - near > std::numeric_limits<float>::epsilon() && "Far plane cannot be less or equal to the near plane!");

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
        assert(right - left > std::numeric_limits<float>::epsilon() && "Right plane cannot be less or equal to the left plane!");
        assert(top - bottom > std::numeric_limits<float>::epsilon() && "Top plane cannot be less or equal to the bottom plane!");
        assert(far - near > std::numeric_limits<float>::epsilon() && "Far plane cannot be less or equal to the near plane!");

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
        assert(far - near > std::numeric_limits<float>::epsilon() && "Far plane cannot be less or equal to the near plane!");

        projectionMatrix = glm::perspective(fov, aspect, near, far);

        // https://github.com/g-truc/glm/blob/0.9.5/glm/gtc/matrix_transform.inl#L208
        // TODO(Dory): Fix this, I don't know where this bug comes from, but it's probably in the code somewhere, it's not GLM
        projectionMatrix[2][2] *= -1.0f;
        projectionMatrix[2][3] *= -1.0f;
        projectionMatrix[3][2] *= 2.0f;
    }

    void Camera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
        viewMatrix = glm::lookAt(position, target, up);
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

        inverseViewMatrix = glm::mat4{1.0f};
        inverseViewMatrix[0][0] = u.x;
        inverseViewMatrix[0][1] = u.y;
        inverseViewMatrix[0][2] = u.z;
        inverseViewMatrix[1][0] = v.x;
        inverseViewMatrix[1][1] = v.y;
        inverseViewMatrix[1][2] = v.z;
        inverseViewMatrix[2][0] = w.x;
        inverseViewMatrix[2][1] = w.y;
        inverseViewMatrix[2][2] = w.z;
        inverseViewMatrix[3][0] = position.x;
        inverseViewMatrix[3][1] = position.y;
        inverseViewMatrix[3][2] = position.z;
    }

    void Camera::setViewXYZ(glm::vec3 position, glm::vec3 rotation) {
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);

        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);

        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);

        const float c1s2 = c1 * s2;
        const float s1s2 = s1 * s2;

        const glm::vec3 u{s1s2 * s3 + c1 * c3, c2 * s3, c1s2 * s3 - s1 * c3};
        const glm::vec3 v{s1s2 * c3 - c1 * s3, c2 * c3, c1s2 * c3 + s1 * s3};
        const glm::vec3 w{s1 * c2, -s2, c1 * c2};

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

        inverseViewMatrix = glm::mat4{1.0f};
        inverseViewMatrix[0][0] = u.x;
        inverseViewMatrix[0][1] = u.y;
        inverseViewMatrix[0][2] = u.z;
        inverseViewMatrix[1][0] = v.x;
        inverseViewMatrix[1][1] = v.y;
        inverseViewMatrix[1][2] = v.z;
        inverseViewMatrix[2][0] = w.x;
        inverseViewMatrix[2][1] = w.y;
        inverseViewMatrix[2][2] = w.z;
        inverseViewMatrix[3][0] = position.x;
        inverseViewMatrix[3][1] = position.y;
        inverseViewMatrix[3][2] = position.z;
    }
}