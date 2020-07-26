#pragma once

#include <engine/transform.h>
#include <glm.hpp>

namespace annileen
{
    class Transform;

    enum CameraClearType
    {
        CameraClearColor,
        CameraClearSkybox
    };

    class Camera
    {
    private:
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_CameraTarget;

    public:
        Transform transform;
        float fieldOfView;
        float nearClip;
        float farClip;

        // Clear types
        CameraClearType clearType;
        glm::vec3 clearColor;

        void updateMatrices();

        void setForward(const glm::vec3& forward);

        glm::vec3 getForward() const;
        glm::vec3 getRight() const;
        glm::vec3 getUp() const;

        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();
        glm::mat4 getViewProjectionMatrix();

        float* getViewMatrixFloatArray();
        float* getProjectionMatrixFloatArray();
        float* getViewProjectionMatrixFloatArray();

        Camera(float fov, float nearc, float farc);
        ~Camera();
    };
}
