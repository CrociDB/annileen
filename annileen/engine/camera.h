#pragma once

#include <engine/transform.h>
#include <glm.hpp>

namespace annileen
{
    class Transform;

    enum class CameraClearType
    {
        CameraClearColor,
        CameraClearSkybox,
        CameraClearNone
    };

    class Camera
    {
    private:
        Transform m_Transform;

        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_CameraTarget;

        float m_FieldOfView;
        float m_NearClip;
        float m_FarClip;
    public:
        inline Transform& transform() { return m_Transform; }

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
