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

        const float tanHalfFov = 1.0f / glm::tan(fov / 2.0f);
        const float ffn = far / (far - near);

        projectionMatrix = glm::mat4{0.0f};

        projectionMatrix[0][0] = tanHalfFov / aspect;
        projectionMatrix[1][1] = tanHalfFov;
        projectionMatrix[2][2] = ffn;
        projectionMatrix[2][3] = 1.0f;
        projectionMatrix[3][2] = near * -ffn;
    }

    void Camera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
        const glm::vec3 w{glm::normalize(direction)};
        const glm::vec3 u{glm::normalize(glm::cross(w, up))};
        const glm::vec3 v{glm::cross(w, u)};

        viewMatrix = glm::mat4{{u.x, v.x, w.x, 0.0f},
                            {u.y, v.y, w.y, 0.0f},
                            {u.z, v.z, w.z, 0.0f},
                            {-glm::dot(u, position),-glm::dot(v, position),-glm::dot(w, position),1.0f}
        };
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

        viewMatrix = glm::mat4{{u.x, v.x, w.x, 0.0f},
                               {u.y, v.y, w.y, 0.0f},
                               {u.z, v.z, w.z, 0.0f},
                               {-glm::dot(u, position),-glm::dot(v, position),
                                -glm::dot(w, position),1.0f}
        };
    }
}