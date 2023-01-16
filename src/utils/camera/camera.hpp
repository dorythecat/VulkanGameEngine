#ifndef CAMERA_HPP
#define CAMERA_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include <cassert>
#include <limits>

namespace Engine {
    class Camera {
    public:
        void setOrthographicProjection (float left, float right, float top, float bottom, float near, float far);
        void setPerspectiveProjection (float fov, float aspect, float near, float far);

        void setViewDirection(glm::vec3 position,
                              glm::vec3 direction,
                              glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});
        void setViewTarget(glm::vec3 position,
                           glm::vec3 target,
                           glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});
        void setViewXYZ(glm::vec3 position, glm::vec3 rotation);

        glm::mat4 getProjectionMatrix () const { return projectionMatrix; }
        glm::mat4 getViewMatrix () const { return viewMatrix; }

    private:
        glm::mat4 projectionMatrix{1.0f};
        glm::mat4 viewMatrix{1.0f};
    };
}

#endif